// pkgcata.cpp : Defines the entry point for the console application.
//

/*

## Environments

Most times, there is only one set of libraries.  All your programs
link to them.

But of course, you might have to work on the upgrade of one
package used by a program.  During that time, you also need 
to have a working environment for the regular program still
using the previous version of a package.  This is where
environments come into play.

Package catalog finds the package files in a path relative
to its executable location.  Let us suppose there is a directory
called "pkgcata" in /home/phil.  We say "there is an environment
at /home/phil/pkgcata":

```

/
    home/
        phil/
            pkgcata/
                pkgcata    # the executable
                packages/
                    zip/
                        include/
                            zip.h
                        lib/
                            libzip.a
                        zip.pkg
                    curl/
                        include/
                            curl.h
                        lib/
                            libcurl.a
                        curl.pkg
                    yaml-cpp/
                        yaml-cpp.pkg
```

In packages, each package has a directory.  In each package
directory there is a file ending with the extension pkg.  Those
file contain all the details to link with the package: header 
paths, library variants (location and related compilation options), 
tools, data files, etc.  We can see that the zip and curl packages
are actually hosted in the /home/phil/pkgcata environment.  On
the other hand, package yaml-cpp must point to files somewhere
else on the filesystem. Even so, this yaml-cpp package is part
of the /home/phil/pkgcata enevironment.

This way, various environments can be made to suit various 
situations.  The build system is tols which environment to use
by telling it which pkgcata executable to use.

This scheme allows for package files to point to system libraries
too.

In the future, we can imagine that one environment could be 
configured to "derive" from another environment.  A single
confguration file beside the pkgcata executable could
contain one or more paths to other "base" environments.
*/

#include "stdafx.h"

#include <cstdlib>
#include <vector>
#include <filesystem>

#include <yaml-cpp/yaml.h>

namespace future_filesystem = std::experimental::filesystem::v1;

class auto_malloc
{
public:
    explicit auto_malloc(void * buf) noexcept
        : buf_(buf) {}

    ~auto_malloc()
    { free(buf_); }

private:
    void * buf_;
};

namespace pkgcata
{

    using future_filesystem::path;
    using future_filesystem::directory_iterator;
    using std::vector;
    using std::string;

    class exception : public std::exception
    {};

    class library
    {
    public:
        path header_path() const;
        path path() const;
    };

    // OS-specific places where catalogs can be found
    vector<path> catalog_places()
    {
        char * buf = NULL;
        if (_dupenv_s(&buf, NULL, "APPDATA"))
        {
            throw exception();
        }

        auto_malloc safe_buf(buf);

        string appdata_env{ buf };

        return { path(appdata_env) / "pkgcata" };
    }

    class catalog
    {
    public:
        catalog();
        void find(string const & name, library & out) const;
    private:
        vector<path> places_;
    };

    catalog::catalog()
    {
        places_ = catalog_places();
    }

    void catalog::find(string const & name, library & out) const
    {
        for (auto const & p : places_)
        {
            for (auto e : directory_iterator(p))
            {
                // if e match ".pgk" filename
                // read it, parse it, 
                // if correct library, 
            }
        }
    }
}

int main()
{
    pkgcata::catalog cata;

    pkgcata::library yamllib;
    cata.find("yaml-cpp", yamllib);

    YAML::Node config = YAML::LoadFile("config.yaml");
 
    return 0;
}

