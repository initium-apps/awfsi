#include"./entry.h"

#if defined(AWFSI_POSIX)
    #include<sys/stat.h>
    #include<dirent.h>
    #include<unistd.h>
#else
    #error Not yet implemented
#endif

#include<cstring>

namespace fs {

entry::entry(fs::str path) : path(path) {
}

entry::~entry() {
}

bool entry::exists() {
    #if defined(AWFSI_POSIX)
        static thread_local struct stat sb;
        return stat(path.c_str(), &sb) == 0;
    #else
        #error Not yet implemented
    #endif
}

bool entry::mkdir() {
    #if defined(AWFSI_POSIX)
        return ::mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) == 0;
    #else
        #error Not yet implemented
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
        #error Not yet implemented
    #endif
}

bool entry::is_dir() {
    #if defined(AWFSI_POSIX)
        static thread_local struct stat sb;

        return stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode);
    #else
        #error Not yet implemented
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
        #error Not yet implemented.
    #endif
}

const fs::str& entry::get_path() const {
    return this->path;
}

const char* entry::c_path() const {
    return this->path.c_str();
}

fs::str entry::get_name() const {
    return path.substr(path.find_last_of(fs::sep) + 1);
}

fs::str entry::get_ext(bool first) const {
    fs::str name = get_name();
    if(first)
        return name.substr(0, name.find(fs::ext_sep));
    else
        return name.substr(0, name.find_last_of(fs::ext_sep));
}

std::ostream& operator<<(std::ostream& strm, const fs::entry& e) {
    return strm << e.path;
}


} // namespace fs
