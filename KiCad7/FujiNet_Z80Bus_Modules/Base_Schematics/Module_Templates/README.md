# New Module setup

Open the Modules project in this folder and SAVE AS "name" in the folder level above. Then open and SAVE AS the Module_Project_impl into the new Modules project (make a nested project).

A module has a project to manage the library paths. Some modules will need parts in the FujiNet Library. The nested project should have the same name as the module with _impl on the end. This sub-project is for creating an implementation of the module. Ideally, the PCB should have connectors/pin headers to enable testing of the module.