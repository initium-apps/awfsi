#ifndef FS_ENTRY_H
#define FS_ENTRY_H

#if defined(_WIN32)
    #define AWFSI_WIN
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
    #define AWFSI_POSIX
#endif

#include<string>
#include<iostream>
#include<vector>

namespace fs {

#if defined(AWFSI_WIN)
    typedef std::wstring str;

    constexpr char sep = '\\';
#elif defined(AWFSI_POSIX)
    typedef std::string str;

    constexpr char sep = '/';
#endif

class entry {
    fs::str path;

    public:
        entry(fs::str);
        ~entry();

        bool exists();
        bool mkdir();
        bool mkdirs();

        fs::entry get_parent();

        bool get_children(std::vector<fs::entry>&);

        const fs::str& get_path() const;

        friend std::ostream& operator<<(std::ostream&, const entry&);
};

} // namespace fs

#endif // FS_ENTRY_H
