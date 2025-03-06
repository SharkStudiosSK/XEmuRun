#include "archive.h"
#include <iostream>
#include <filesystem>
#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <cstring>

namespace fs = std::filesystem;

namespace XEmuRun {

bool extractArchive(const std::string& archivePath, const std::string& outputDir) {
    struct archive* a;
    struct archive* ext;
    struct archive_entry* entry;
    int flags;
    int r;

    // Select which attributes we want to restore.
    flags = ARCHIVE_EXTRACT_TIME;
    flags |= ARCHIVE_EXTRACT_PERM;
    flags |= ARCHIVE_EXTRACT_ACL;
    flags |= ARCHIVE_EXTRACT_FFLAGS;

    a = archive_read_new();
    archive_read_support_format_all(a);
    archive_read_support_filter_all(a);
    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);

    if ((r = archive_read_open_filename(a, archivePath.c_str(), 10240))) {
        std::cerr << "Error opening archive: " << archive_error_string(a) << std::endl;
        return false;
    }

    // Create output directory if it doesn't exist
    if (!fs::exists(outputDir)) {
        try {
            fs::create_directories(outputDir);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Failed to create output directory: " << e.what() << std::endl;
            return false;
        }
    }

    // Extract each entry
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r != ARCHIVE_OK) {
            std::cerr << "Error reading archive header: " << archive_error_string(a) << std::endl;
            return false;
        }

        // Prepare full output path
        std::string entryPath = archive_entry_pathname(entry);
        std::string fullOutputPath = (fs::path(outputDir) / entryPath).string();
        
        // Update the entry with the new path
        archive_entry_set_pathname(entry, fullOutputPath.c_str());

        r = archive_write_header(ext, entry);
        if (r != ARCHIVE_OK) {
            std::cerr << "Error writing header: " << archive_error_string(ext) << std::endl;
        } else {
            // Copy the file data if it's a regular file
            if (archive_entry_size(entry) > 0) {
                const void* buff;
                size_t size;
                la_int64_t offset;

                while ((r = archive_read_data_block(a, &buff, &size, &offset)) != ARCHIVE_EOF) {
                    if (r != ARCHIVE_OK) {
                        std::cerr << "Error reading data block: " << archive_error_string(a) << std::endl;
                        break;
                    }
                    r = archive_write_data_block(ext, buff, size, offset);
                    if (r != ARCHIVE_OK) {
                        std::cerr << "Error writing data block: " << archive_error_string(ext) << std::endl;
                        break;
                    }
                }
            }
        }
        
        r = archive_write_finish_entry(ext);
        if (r != ARCHIVE_OK) {
            std::cerr << "Error finishing entry: " << archive_error_string(ext) << std::endl;
            return false;
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);
    
    return true;
}

bool createArchive(const std::string& directoryPath, const std::string& outputArchive) {
    struct archive* a;
    struct archive_entry* entry;
    struct stat st;
    char buff[8192];
    int len;
    int fd;

    a = archive_write_new();
    archive_write_set_format_zip(a);
    archive_write_open_filename(a, outputArchive.c_str());

    if (!fs::exists(directoryPath)) {
        std::cerr << "Source directory does not exist: " << directoryPath << std::endl;
        return false;
    }

    // Get the parent directory of the source to calculate relative paths
    fs::path sourceDirPath = fs::absolute(directoryPath);

    try {
        // Recursively add all files and directories
        for (const auto& entry_path : fs::recursive_directory_iterator(sourceDirPath)) {
            std::string fullPath = entry_path.path().string();
            std::string relativePath = entry_path.path().lexically_relative(sourceDirPath).string();

            // Skip if entry is a directory
            if (fs::is_directory(entry_path)) {
                continue;
            }

            // Get file info
            if (stat(fullPath.c_str(), &st) != 0) {
                std::cerr << "Failed to stat file: " << fullPath << std::endl;
                continue;
            }

            // Create a new entry
            entry = archive_entry_new();
            archive_entry_set_pathname(entry, relativePath.c_str());
            archive_entry_set_size(entry, st.st_size);
            archive_entry_set_filetype(entry, AE_IFREG);
            archive_entry_set_perm(entry, 0644);
            archive_write_header(a, entry);

            // Write file data to archive
            fd = open(fullPath.c_str(), O_RDONLY);
            if (fd < 0) {
                std::cerr << "Failed to open file for reading: " << fullPath << std::endl;
                archive_entry_free(entry);
                continue;
            }

            while ((len = read(fd, buff, sizeof(buff))) > 0) {
                archive_write_data(a, buff, len);
            }

            close(fd);
            archive_entry_free(entry);
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
        archive_write_close(a);
        archive_write_free(a);
        return false;
    }

    archive_write_close(a);
    archive_write_free(a);
    
    return true;
}

} // namespace XEmuRun
