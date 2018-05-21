#include"./entry.h"

#if defined(AWFSI_POSIX)
    #include<sys/stat.h>
    #include<dirent.h>
    #include<unistd.h>
#else
    #include<windows.h>
    #include<wchar.h>
#endif

#include<cstring>

// Used for entry::entry(const char*).
static std::wstring convertStrToWstr(std::string s) {
    return std::wstring(s.begin(), s.end());
}

namespace fs {

entry::entry(const char* cstr) : path(convertStrToWstr(cstr)) {
}

entry::entry(fs::str path) : path(path) {
}

entry::~entry() {
}

bool entry::exists() {
    #if defined(AWFSI_POSIX)
        static thread_local struct stat sb;
        return stat(path.c_str(), &sb) == 0;
    #else
        DWORD w = GetFileAttributesW(path.c_str());
        return w != INVALID_FILE_ATTRIBUTES;
    #endif
}

bool entry::mkdir() {
    #if defined(AWFSI_POSIX)
        return ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
    #else
        return ::_wmkdir(path.c_str());
    #endif
}

bool entry::mkdirs() {
    fs::entry guardian = get_parent();
    if(!guardian.exists()) {
        if(!guardian.mkdirs()) return false;
    } else if(!exists()) {
        return this->mkdir();
    }
    return true;
}

bool entry::remove() {
    #if defined(AWFSI_POSIX)
        if(is_dir()) {
            std::vector<fs::entry> children;
            get_children(children);
            for(fs::entry& e : children) {
                e.remove();
            }

            return rmdir(path.c_str()) == 0;
        } else {
            return unlink(path.c_str()) == 0;
        }
    #else
        // For some god-awful reason, strings passed to
        // SHFileOperation need to be double null-terminated.
        fs::str cpy(path);
        cpy.push_back('\0');
        struct _SHFILEOPSTRUCTW shfo = {
            NULL,
            FO_DELETE,
            cpy.c_str(),
            NULL,
            FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
            FALSE,
            NULL,
            NULL
        };
        int ret = SHFileOperationW(&shfo);
        cpy.pop_back();
        return ret == 0;
    #endif
}

bool entry::is_dir() {
    #if defined(AWFSI_POSIX)
        static thread_local struct stat sb;

        return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
    #else
        DWORD fileAttrs = GetFileAttributesW(path.c_str());
        return (fileAttrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
    #endif
}

fs::entry entry::get_parent() {
    return fs::entry(path.substr(0, path.find_last_of(fs::sep)));
}

bool entry::get_children(std::vector<fs::entry>& ret) {
    #if defined(AWFSI_POSIX)
        struct dirent *ent;

        DIR *dp;
        dp = opendir(path.c_str());
        if (dp == NULL) {
            return false;
        }

        while ((ent = readdir(dp))) {
            if(std::strcmp(ent->d_name, ".") != 0 && std::strcmp(ent->d_name, "..") != 0) {
                ret.push_back(fs::entry(this->path + fs::sep + ent->d_name));
            }
        }

        closedir(dp);
        return true;
    #else
        HANDLE hFind;
        WIN32_FIND_DATAW data;

        hFind = FindFirstFileW((path + L"\\*").c_str(), &data);
        if(hFind != INVALID_HANDLE_VALUE) {
            do {
                if(wcscmp(data.cFileName, L".") != 0 && wcscmp(data.cFileName, L"..") != 0) {
                    ret.push_back(fs::str(path + static_cast<wchar_t>(fs::sep) + data.cFileName));
                }
            } while(FindNextFileW(hFind, &data));
        } else {
            return false;
        }

        return true;
    #endif
}

const fs::str& entry::get_path() const {
    return this->path;
}

const fs::str::value_type* entry::c_path() const {
    return this->path.c_str();
}

fs::str entry::get_name() const {
    return path.substr(path.find_last_of(fs::sep) + 1);
}

fs::str entry::get_stem(bool first) const {
    fs::str name = get_name();
    return name.substr(0, name.size() - get_ext(first).size() - 1);
}

fs::str entry::get_ext(bool first) const {
    fs::str name = get_name();
    if(first)
        return name.substr(name.find(fs::ext_sep) + 1);
    else
        return name.substr(name.find_last_of(fs::ext_sep) + 1);
}

std::ostream& operator<<(std::ostream& strm, const fs::entry& e) {
    return strm << e.path;
}


} // namespace fs
