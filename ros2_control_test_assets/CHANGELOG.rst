^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package ros2_control_test_assets
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

5.4.0 (2025-07-21)
------------------

5.3.0 (2025-07-02)
------------------

5.2.0 (2025-06-07)
------------------

5.1.0 (2025-05-24)
------------------
* Read `data_type` for all types of interfaces (`#2235 <https://github.com/ros-controls/ros2_control/issues/2235>`_)
* Contributors: Sai Kishor Kothakota

5.0.0 (2025-05-21)
------------------

4.29.0 (2025-05-04)
-------------------
* [Diagnostics] Add diagnostics of execution time and periodicity of the hardware components (`#2086 <https://github.com/ros-controls/ros2_control/issues/2086>`_)
* Contributors: Sai Kishor Kothakota

4.28.1 (2025-04-17)
-------------------

4.28.0 (2025-04-10)
-------------------
* Integrate joint limit enforcement into `ros2_control` framework functional with Async controllers and components  (`#2047 <https://github.com/ros-controls/ros2_control/issues/2047>`_)
* Improve package descriptions & update maintainers (`#2103 <https://github.com/ros-controls/ros2_control/issues/2103>`_)
* Contributors: Bence Magyar, Sai Kishor Kothakota

4.27.0 (2025-03-01)
-------------------
* [CM] Fix the controller deactivation on the control cycles returning ERROR  (`#1756 <https://github.com/ros-controls/ros2_control/issues/1756>`_)
* Contributors: Sai Kishor Kothakota

4.26.0 (2025-02-07)
-------------------

4.25.0 (2025-01-29)
-------------------

4.24.0 (2025-01-13)
-------------------

4.23.0 (2024-12-29)
-------------------
* Move test_utils module from demos repo (`#1955 <https://github.com/ros-controls/ros2_control/issues/1955>`_)
* Contributors: Christoph Fröhlich

4.22.0 (2024-12-20)
-------------------
* Async Hardware Components (`#1567 <https://github.com/ros-controls/ros2_control/issues/1567>`_)
* Let sensors also export state interfaces of joints (`#1885 <https://github.com/ros-controls/ros2_control/issues/1885>`_)
* Contributors: Christoph Fröhlich, Sai Kishor Kothakota

4.21.0 (2024-12-06)
-------------------
* [Feature] Choose different read and write rate for the hardware components (`#1570 <https://github.com/ros-controls/ros2_control/issues/1570>`_)
* Contributors: Sai Kishor Kothakota

4.20.0 (2024-11-08)
-------------------
* Add Support for SDF (`#1763 <https://github.com/ros-controls/ros2_control/issues/1763>`_)
* Contributors: Aarav Gupta

4.19.0 (2024-10-26)
-------------------

4.18.0 (2024-10-07)
-------------------
* Automatic Creation of Handles in HW, Adding Getters/Setters (variant support) (`#1688 <https://github.com/ros-controls/ros2_control/issues/1688>`_)
* Contributors: Manuel Muth

4.17.0 (2024-09-11)
-------------------
* [HWItfs] Add key-value-storage to the InterfaceInfo (`#1421 <https://github.com/ros-controls/ros2_control/issues/1421>`_)
* Contributors: Manuel Muth

4.16.1 (2024-08-24)
-------------------

4.16.0 (2024-08-22)
-------------------

4.15.0 (2024-08-05)
-------------------

4.14.0 (2024-07-23)
-------------------
* Unused header cleanup (`#1627 <https://github.com/ros-controls/ros2_control/issues/1627>`_)
* Contributors: Henry Moore

4.13.0 (2024-07-08)
-------------------
* [ControllerChaining] Export state interfaces from chainable controllers (`#1021 <https://github.com/ros-controls/ros2_control/issues/1021>`_)
* Remove mimic parameter from ros2_control tag (`#1553 <https://github.com/ros-controls/ros2_control/issues/1553>`_)
* Contributors: Christoph Fröhlich, Sai Kishor Kothakota

4.12.0 (2024-07-01)
-------------------
* [RM] Rename `load_urdf` method to `load_and_initialize_components` and add error handling there to avoid stack crashing when error happens. (`#1354 <https://github.com/ros-controls/ros2_control/issues/1354>`_)
* [Feature] Hardware Components Grouping (`#1458 <https://github.com/ros-controls/ros2_control/issues/1458>`_)
* Contributors: Dr. Denis, Sai Kishor Kothakota

4.11.0 (2024-05-14)
-------------------
* Parse URDF soft_limits into the HardwareInfo structure (`#1488 <https://github.com/ros-controls/ros2_control/issues/1488>`_)
* Contributors: adriaroig

4.10.0 (2024-05-08)
-------------------
* Parse URDF joint hard limits into the HardwareInfo structure (`#1472 <https://github.com/ros-controls/ros2_control/issues/1472>`_)
* Contributors: Sai Kishor Kothakota

4.9.0 (2024-04-30)
------------------
* Component parser: Get mimic information from URDF (`#1256 <https://github.com/ros-controls/ros2_control/issues/1256>`_)
* Contributors: Christoph Fröhlich

4.8.0 (2024-03-27)
------------------

4.7.0 (2024-03-22)
------------------

4.6.0 (2024-03-02)
------------------

4.5.0 (2024-02-12)
------------------

4.4.0 (2024-01-31)
------------------

4.3.0 (2024-01-20)
------------------
* [ResourceManager] adds test for uninitialized hardware (`#1243 <https://github.com/ros-controls/ros2_control/issues/1243>`_)
* Contributors: Maximilian Schik

4.2.0 (2023-12-12)
------------------

4.1.0 (2023-11-30)
------------------

4.0.0 (2023-11-21)
------------------

3.21.0 (2023-11-06)
-------------------

3.20.0 (2023-10-31)
-------------------
* [ResourceManager] deactivate hardware from read/write return value (`#884 <https://github.com/ros-controls/ros2_control/issues/884>`_)
* Contributors: Felix Exner (fexner)

3.19.1 (2023-10-04)
-------------------

3.19.0 (2023-10-03)
-------------------

3.18.0 (2023-08-17)
-------------------

3.17.0 (2023-08-07)
-------------------

3.16.0 (2023-07-09)
-------------------

3.15.0 (2023-06-23)
-------------------

3.14.0 (2023-06-14)
-------------------
* [URDF Parser] Allow empty urdf tag, e.g., parameter (`#1017 <https://github.com/ros-controls/ros2_control/issues/1017>`_)
* Contributors: Felix Exner (fexner)

3.13.0 (2023-05-18)
-------------------

3.12.2 (2023-04-29)
-------------------

3.12.1 (2023-04-14)
-------------------

3.12.0 (2023-04-02)
-------------------

3.11.0 (2023-03-22)
-------------------

3.10.0 (2023-03-16)
-------------------

3.9.1 (2023-03-09)
------------------

3.9.0 (2023-02-28)
------------------

3.8.0 (2023-02-10)
------------------
* Fix CMake install so overriding works (`#926 <https://github.com/ros-controls/ros2_control/issues/926>`_)
* Contributors: Tyler Weaver

3.7.0 (2023-01-24)
------------------

3.6.0 (2023-01-12)
------------------

3.5.1 (2023-01-06)
------------------

3.5.0 (2022-12-06)
------------------
* Rename class type to plugin name #api-breaking #abi-breaking (`#780 <https://github.com/ros-controls/ros2_control/issues/780>`_)
* Contributors: Bence Magyar

3.4.0 (2022-11-27)
------------------

3.3.0 (2022-11-15)
------------------

3.2.0 (2022-10-15)
------------------

3.1.0 (2022-10-05)
------------------

3.0.0 (2022-09-19)
------------------

2.15.0 (2022-09-19)
-------------------

2.14.0 (2022-09-04)
-------------------

2.13.0 (2022-08-03)
-------------------

2.12.1 (2022-07-14)
-------------------

2.12.0 (2022-07-09)
-------------------

2.11.0 (2022-07-03)
-------------------
* Update maintainers of packages (`#753 <https://github.com/ros-controls/ros2_control/issues/753>`_)
* Remove ament autolint (`#749 <https://github.com/ros-controls/ros2_control/issues/749>`_)
* Contributors: Bence Magyar

2.10.0 (2022-06-18)
-------------------
* Make RHEL CI happy! (`#730 <https://github.com/ros-controls/ros2_control/issues/730>`_)
* CMakeLists cleanup (`#733 <https://github.com/ros-controls/ros2_control/issues/733>`_)
* Contributors: Andy Zelenak, Márk Szitanics

2.9.0 (2022-05-19)
------------------

2.8.0 (2022-05-13)
------------------

2.7.0 (2022-04-29)
------------------

2.6.0 (2022-04-20)
------------------

2.5.0 (2022-03-25)
------------------

2.4.0 (2022-02-23)
------------------

2.3.0 (2022-02-18)
------------------

2.2.0 (2022-01-24)
------------------
* Resource Manager API changes for hardware lifecycle #api-breaking #abi-breaking (`#589 <https://github.com/ros-controls/ros2_control/issues/589>`_)
  * Towards selective starting and stoping of hardware components. Cleaning and renaming.
  * Move Lifecycle of hardware component to the bottom for better overview.
  * Use the same nomenclature as for controllers. 'start' -> 'activate'; 'stop' -> 'deactivate'
  * Add selective starting and stopping of hardware resources.
  Add HardwareComponentInfo structure in resource manager.
  Use constants for HW parameters in tests of resource_manager.
  Add list hardware components in CM to get details about them and check their status.
  Use clear name for 'guard' and move release cmd itfs for better readability.
  RM: Add lock for accesing maps with stored interfaces.
  Separate hardware components-related services after controllers-related services.
  Add service for activate/deactive hardware components.
  Add activation and deactivation through ResourceStorage. This helps to manage available command interfaces.
  * Use lifecycle_msgs/State in ListHardwareCompoents for state representation.
  * Simplify repeatable code in methods.
  * Add HW shutdown structure into ResouceManager.
  * Fill out service callback in CM and add parameter for auto-configure.
  * Move claimed_command_itf_map to ResourceStorage from ResourceManager.
  * Do not automatically configure hardware in RM.
  * Lifecycle and claiming in Resource Manager is working.
  * Extend controller manager to support HW lifecycle.
  * Add also available and claimed status into list components service output.
  * Add SetHardwareComponentState service.
  * Make all output in services debug-output.
  * Remove specific services for hardware lifecycle management and leave only 'set_hardware_component_state' service.
  * Make init_resource_manager less stateful.
  * Keep old api to start/activate all components per default.
  * Remove 'moving'/'non-moving' interface-handling.
  * Remove obsolete 'import_components' methods without hardware info and fix post_initialization test.
  Co-authored-by: Bence Magyar <bence.magyar.robotics@gmail.com>
* Contributors: Denis Štogl

2.1.0 (2022-01-11)
------------------

2.0.0 (2021-12-29)
------------------
* Adding support for 'initial_value' parameter. (`#593 <https://github.com/ros-controls/ros2_control/issues/593>`_)
* Contributors: bailaC

1.2.0 (2021-11-05)
------------------

1.1.0 (2021-10-25)
------------------
* Moving diffbot_urdf to ros2_control_test_assets. (`#558 <https://github.com/ros-controls/ros2_control/issues/558>`_)
* Contributors: bailaC

1.0.0 (2021-09-29)
------------------
* Remove unnecessary includes (`#518 <https://github.com/ros-controls/ros2_control/issues/518>`_)
* Do not manually set C++ version to 14 (`#516 <https://github.com/ros-controls/ros2_control/issues/516>`_)
* Contributors: Bence Magyar, Denis Štogl

0.8.0 (2021-08-28)
------------------
* Use clang format as code formatter (`#491 <https://github.com/ros-controls/ros2_control/issues/491>`_)
* Transmission parsing v2 (`#471 <https://github.com/ros-controls/ros2_control/issues/471>`_)
* Added GPIO parsing and test (`#436 <https://github.com/ros-controls/ros2_control/issues/436>`_)
* Contributors: Bence Magyar, Denis Štogl, Mathias Arbo

0.7.1 (2021-06-15)
------------------

0.7.0 (2021-06-06)
------------------

0.6.1 (2021-05-31)
------------------

0.6.0 (2021-05-23)
------------------

0.5.0 (2021-05-03)
------------------

0.4.0 (2021-04-07)
------------------
* [ros2_control_test_assets] Fix typo (`#371 <https://github.com/ros-controls/ros2_control/issues/371>`_)
* Contributors: Yutaka Kondo

0.3.0 (2021-03-21)
------------------

0.2.1 (2021-03-02)
------------------

0.2.0 (2021-02-26)
------------------
* Add "Fake" components for simple integration of framework (`#323 <https://github.com/ros-controls/ros2_control/issues/323>`_)
* Moved example URDFs for parser/scenario tests to assets package (`#316 <https://github.com/ros-controls/ros2_control/issues/316>`_)
* Contributors: Denis Štogl

0.1.6 (2021-02-05)
------------------
* correct hardware interface validation in resource manager. (`#317 <https://github.com/ros-controls/ros2_control/issues/317>`_)
* Add missing test dep (`#321 <https://github.com/ros-controls/ros2_control/issues/321>`_)
* Contributors: Bence Magyar, Karsten Knese

0.1.5 (2021-02-04)
------------------
* Add missing buildtool dep (`#319 <https://github.com/ros-controls/ros2_control/issues/319>`_)
* Contributors: Bence Magyar

0.1.4 (2021-02-03)
------------------
* Add test assets package (`#289 <https://github.com/ros-controls/ros2_control/issues/289>`_)
* Contributors: Denis Štogl

0.1.3 (2021-01-21)
------------------

0.1.2 (2021-01-06)
------------------

0.1.1 (2020-12-23)
------------------

0.1.0 (2020-12-22)
------------------
