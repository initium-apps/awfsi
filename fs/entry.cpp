#include"./entry.h"

#if defined(AWFSI_POSIX)
    #include<sys/stat.h>
    #include<dirent.h>
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

std::ostream& operator<<(std::ostream& strm, const fs::entry& e) {
    return strm << e.path;
}


} // namespace fs
