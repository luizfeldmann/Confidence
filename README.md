# Confidence

Confidence is a tool to centralize and manage configuration files.

Its goal is to provide a systematic, structured and comprehensive way to define, maintain and document configuration files, parameters, relations, and variations on a software project.

## Principles & Motivation

The following remarks are the basis for the design and implementation of this tool:

- **Centralization:** All the parameters are defined in a single place, and then exported to whatever format your application requires.
    - Because everything exists within the same domain, it's easy to create logical relationships and rules to define configuration.
    - You won't need to worry about information *redundancy* or *consistency* across multiple configuration files.
    - You only have to manage one global configuration, as opposed to many files.

- **Self-documentation:** Configuration means variation, change and possibility. Documenting it is inherently time consuming, because it's never static by nature. 
    - Documentation should be reader-friendly, and should exist outside the (possibly many) configuration files your project requires. Comments spread sparsely over the project are not good enough.
    - The process of documenting should be tied with the process of writing and maintaining configuration, so as to guarantee consistency and completeness. 
        - It should be impossible to create a parameter without creating the associated documentation. 
        - It should be impossible to update a parameter without updating the documentation.
        - It should be impossible to produce documentation that does not match the configuration parameters, relationships and variations.
    - The only way to obtain this level of correctness is by automation.

- **Versionability:** You should be able to compare versions and keep a history of your global configuration and it's associated documentation.
    - This requires your configuration and documentation to be stored in a comprehensible, editable, diff-able, merge-able, text file.
    - MS Word and PDF documents do **NOT** meet those requirements.

- **Presets:** The global configuration should be viewed as a tree, where most parameters are related to some important few.
    - There should be only a limited number of actual input parameters to your configurations.
    - Those inputs should compound presets, each associated to a specific scenario where the configuration is **substantially** different.
    - When managing configuration, you should be able to realize the scope of your changes. How other scenarios/presets are or aren't affected, and how it impacts the project in the big-picture.
    - The big-picture view should be readily available. Having to manually track dependencies and relations will inevitably lead to oversights.

## Implementation & Features

The *Confidence* tool is implemented in accordance to the principles outlined above in the following manner:

- **Project File:** All the configuration resides within a single project file in the *JSON* format. This file should be as self-contained as possible, requiring few or none outside sources.

- **Documentation Exporters:** The tool can generate *.HTML* and *.MD* documentation from the project file.

- **Presets:** The resulting output is evaluated from the combination of the following constructs:
    - **Configuration:** A named preset that can be used as a key to define rules logic for other parameters. The user executes a single chosen *configuration* at a time.
    - **Instances:** Each component of the project may be executed multiple times with small output variations, allowing reuse of logic.

- **Constants:** User-defined expressions that are independent from the *instance* or *configuration* being executed.

- **Variables:** User-defined expressions that are chosen from a table keyed by *instance* and *configuration*. Allows creation of comprehensible rules of selection for parameters. The chosen values for the variables may latter be reused in the project's logic.

- **Conditionals:** Performs logical comparison of expressions and logical association of conditions to determine the execution of certain blocks of the project, allowing for more complex rules for deciding the final configuration.

- **Environment Imports:** Allows importing environmental variables for use in expressions inside the project, as a way to parameterize the generated configuration according to the local system's specificities.

- **File Operations:** Performs I/O on configuration files in formats *.XML* and *.INI* 
    - **Read:** Imports parameters from configuration files into the project dictionary for use in later expressions.
    - **Write:** Updates existing entries on configuration files using the evaluated result of expressions.

- **Run Process:** Executes an arbitrary process with the provided arguments, (optionally) waits for completion and (optionally) checks the return code. This is mainly aimed at allowing the integration of third-party tools in the configuration process.

- **File Generation:** Creates configuration files by replacing placeholder expressions inside a template. This allows handling arbitrary file formats in a generic way, but sacrifices the principle of structured, well-defined steps to produce the final configuration.
    - **Template embedding:** The template text of the file may be contained inside the project. This is consistent with the principle of centralization, but may be impractical for large files.
    - **External templates:** The template text is loaded from an external file. This goes against the principles of centralization and self-containment, but allows for additional logic by dynamically selecting the template file using expressions.

## Usage

A command line interface with basic functionality is available in order to allow integration of Confidence with other tools. Basic usage is as follows:

- Show help: `Confidence.exe --help`
- Show version: `Confidence.exe --version`
- Export documentation: `Confidence.exe --docs path_to_project_file.cfx`
- Run/apply a configuration: `Confidence.exe --run name_of_configuration path_to_project_file.cfx`

## Development Environment

This project welcomes contributions.
Feel free to submit issues and enhancement requests.
Please follow the "fork-and-pull" Git workflow. Be sure to merge the latest from upstream before making a pull request.

### Library dependencies:

- **GUI:** [wxWidgets](https://github.com/wxWidgets/wxWidgets)
- **Command line parsing:** [CLI11](https://github.com/CLIUtils/CLI11)
- **XML file I/O:** [pugixml](https://github.com/zeux/pugixml)
- **INI file I/O** [mINI](https://github.com/pulzed/mINI)
- **JSON file I/O and serialization:** [rapidjson](https://github.com/Tencent/rapidjson) and [JInterface](https://github.com/luizfeldmann/JInterface)
- **Documentation in MD and HTML:** [cmark](https://github.com/commonmark/cmark)
- **Resources packaging:** [cmrc](https://github.com/vector-of-bool/cmrc)
- **Graph visualization:** [Graphviz](https://graphviz.org/)

### Dev & Build tools:

- **Build system:** [CMake](https://cmake.org/)
- **GUI design:** [wxFormBuilder](https://github.com/wxFormBuilder/wxFormBuilder)
- **Building image resources from SVG:** [ImageMagick](https://imagemagick.org/)
- **Exporting this *Readme* to HTML:** [pandoc](https://pandoc.org/)
- **Creation of installer package:** [NSIS](https://nsis.sourceforge.io/)

## Copyright and Licensing

We are pleased to support the open source community by making this software freely available for individuals and businesses.

```
Copyright (C) 2022 Luiz Gustavo Pfitscher e Feldmann.

Licensed under the MIT License (the "License"); you may not use this file except
in compliance with the License. You may obtain a copy of the License at

http://opensource.org/licenses/MIT

Unless required by applicable law or agreed to in writing, software distributed 
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR 
CONDITIONS OF ANY KIND, either express or implied. See the License for the 
specific language governing permissions and limitations under the License.
```
