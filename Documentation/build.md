# Build system

## Introduction 

mdepx's build system is a set of python3 scripts located in mdepx/tools/ directory.

It compiles your application and OS, links all together to the ELF executable.

Files to build, compile flags and macroses to define are described in a config file.

Config file syntax is heavily inspired by libucl and nginx configuration file and has JSON-like structure, hence it is fully convertable to JSON.

#### Main principles of configuration file

* Everything in the config is 'key value' terminated with a colon ';'.
* Value is a list of strings or a dict (a context leaf).
* Same key could be presented many times, and all its values are appended to a list by order of parsing.
* A context leaf inherits properties (options / compile flags) from its parent.

### Basic structure

    key value;

    value {
        key val1 val2;
        key val3;

        # comment
        param 3;

        val1 {
            k v;
        };

        val2 {
            key value;
            value {
                    a b;
            };
        };
    };

It will be converted to JSON as:

    {
        'key': ['value'],
        'value': {
            'key': ['val1', 'val2', 'val3'],
            'param': ['3'],
            'val1': {
                'k': ['v']
            },
            'val2': {
                'key': ['value'],
                'value': {
                    'a': ['b']
                }
            }
        }
    }

#### Invoking a build

In your application directory create a Makefile:

    all:
        @python3 -B mdepx/tools/emitter.py -d mdepx.conf

## Configuration file directives

* * *
Syntax: **module** modname;

A pre-processor directive that:

 * opens module.conf file from the modname directory. If file exists then it puts file's body to the current context leaf;
 * includes a modname context to processing.

* * *
Syntax: **options** optname;

Expands entries from the optname leaf to the current context and includes them to processing.

Example:

    module1 {
        objects obj1.o obj2.o;
        options aaa;

        aaa {
            objects obj3.o;
            key value;
        };
    };

will be transformed to:

    module1 {
        objects obj1.o obj2.o obj3.o;
        key value;
    };

* * *
Syntax: **append-search-path** path [path1 ..];

Adds -Ipath [-Ipath1] to the compiler.

* * *
Syntax: **set-search-path** path [path1 ..];

Replaces search-path inherited from parent context with -Ipath [-Ipath1].

* * *
Syntax: **objects** obj.o [obj1.o ..];

Includes objects to the build (compilation and linking stages). Object names must end with '.o'. The corresponsing '.c' or '.S' file will be passed to a compiler.

* * *
Syntax: **link** ldscript filename;

Invokes linker with provided ldscript as relative path to a linker script. All the compiled objects will be passed to a linker. filename is an output ELF file.

* * *
Syntax: **prefix** string;

Sets initial prefix for macroses that will be passed to a compiler.

Example:

    module1 {
        prefix mdx;
        module aaa;
        options bbb;

        aaa {
            flag 1;
        };

        bbb {
            flag 2;
        };
    };

Macroses will be collected and passed to a compiler as:

    cc -DMDX_AAA -DMDX_AAA_FLAG=1 -DMDX_BBB -DMDX_FLAG=2 ..

Note that macroses are global and will be passed to compiler regardless of object.
