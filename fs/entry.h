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
    constexpr char ext_sep = '.';
#elif defined(AWFSI_POSIX)
    typedef std::string str;

    constexpr char sep = '/';
    constexpr char ext_sep = '.';
#endif

/**
 * The basis of the entire library. Stands for Filesystem Entry, as it represents both files and directories.
 * Is constructed with a single string, that is the path of the entry.
 */
class entry {
    fs::str path; /**< Self-explanatory. */

    public:
        entry(fs::str);
        ~entry();

        bool exists();
        bool mkdir();
        bool mkdirs();
        bool remove();
        bool is_dir();

        fs::entry get_parent();

        /**
         * Non-recursively gathers the children of this entry into the argument.
         * @param vec The std::vector in which the entries representing the children are added.
         * @return true, if success, else false.
         */
        bool get_children(std::vector<fs::entry>& vec);

        /**
         * @return The entry path. This path is not guaranteed to be absolute, relative nor canonical.
         */
        const fs::str& get_path() const;
        const char* c_path() const;

        /**
         * @return The entry name, including the extension.
         */
        fs::str get_name() const;

        /**
         * @param first Handle multi-word extensions?
         * @return The entry extension, excluding the extension-separator (which is a dot).
         */
        fs::str get_ext(bool first = true) const;

        /**
         * @return An instance of
         */

        friend std::ostream& operator<<(std::ostream&, const entry&);
};

} // namespace fs

#endif // FS_ENTRY_H
