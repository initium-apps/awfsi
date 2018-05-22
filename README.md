# AWFSI: an Actually Working FileSystem Interface.
This was made after Boost.Filesystem segfaulted in every single function and StackOverflow was too naive as usual to help.  
Plus Boost.Filesystem has absolutely no documentation, so fuck you both.

## Installation
This is not a header-only library, however it is very small (currently 2 files).  
Simply copy-paste files under the `fs` directory to a common directory in your project.

## Usage
See the [wiki](https://github.com/initium-apps/awfsi/wiki/Usage).

## Planned Features
In no particular order:

 * ~~**_(!)_ Win32 support.**~~
 * Recursive `get_children` function.
 * Actually testing this on OS X.

## License
The code in the `/fs` directory is licensed under the MIT License.
