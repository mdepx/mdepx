# Build system

## Introduction 

mdepx's build system is a set of python3 scripts located in mdepx/tools/ directory.

It compiles your application and OS, links all together to an ELF executable.

Files to build, compile flags and macroses to define are described in a config file.

Config file is heavily inspired by libucl and nginx configuration file and has JSON-like structure, hence it is fully convertable to JSON.

#### Main principles of configuration file

* Everything in the config is 'key value' terminated with a colon ';'.
* Same key could be presented many times, and all its values are appended to a list by order of parsing. A value is always a list after converting to json.
* A context leaf inherits properties (options / flags) from its parent.

## Configuration file directives

```
Syntax: module modname;
```

A pre-processor directive that:

 * opens module.conf file from the modname directory. If file exists then it puts file's body to the current context leaf;
 * includes a modname context to processing.

`
Syntax: options optname;
`

Expands entries from the optname leaf to the current context and includes them to processing.

Example:

    module1 {
        options aaa;

        aaa {
            key value;
        };
    };

will be expanded to:

    module1 {
        key value;
    };


```
Syntax: append-search-path path [path1 ..]
```

Adds -Ipath [-Ipath1] to the compiler.

```
Syntax: set-search-path path [path1 ..]
```

Replaces search-path inherited from parent context with -Ipath [-Ipath1].
