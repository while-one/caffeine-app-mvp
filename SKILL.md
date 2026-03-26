# Skill: Caffeine App Development

This skill provides expert guidance on the architectural standards and development workflows for building applications within the **Caffeine Framework** ecosystem.

## 1. Architectural Layers

Caffeine applications are strictly layered to ensure maximum portability and testability:

1.  **Application Logic (`src/app/`):** Contains the hardware-agnostic business logic, state machines, and system orchestration. It should never include hardware-specific headers (like `cfn_hal_gpio.h`).
2.  **Board Support Package (`src/bsp/`):** Acts as the bridge between the application logic and the hardware. It implements abstract interfaces (e.g., `bsp_led_init()`) using `caffeine-hal` APIs.
3.  **Hardware Abstraction Layer (`caffeine-hal`):** The generic interface that provides a unified way to interact with hardware peripherals across different vendors and architectures.

## 2. Development Workflows

### Adding a New Hardware-Agnostic Feature
1.  Define the feature's interface in `src/app/`.
2.  If the feature requires hardware access, define a corresponding abstract interface in `src/bsp/`.
3.  Implement the feature logic in `src/app/`, calling the abstract BSP functions.

### Adding Support for a New Hardware Target
1.  Define the hardware configuration in `caffeine-hal-ports` by creating a target `.cmake` script (e.g., `cmake/ports/stm32/stm32f4/my_target.cmake`).
2.  Create a new CMake preset in the application's `CMakePresets.json` that sets `CFN_HAL_PORT_TARGET` to your new script.
3.  Update the BSP implementation (e.g., `src/bsp/bsp_led.c`) to map the abstract functions to the target's physical pins.
4.  Use the `build.sh` script to build for the new target preset.

### Overriding Hardware Parameters
You can customize specific hardware parameters (HSE/LSE clock frequencies and VDD voltage) directly in your `CMakePresets.json`:
1.  Add the relevant cache variables to your preset (e.g., `"CFN_HAL_CLOCK_HSE_HZ": "8000000"`).
2.  These values will be injected into the vendor HAL configuration during the generation phase.

### Writing Unit Tests
1.  Place your GTest-based tests in `tests/`.
2.  Enable the global `CFN_BUILD_TESTS` flag to automatically link against the `caffeine::hal-mock` library for host-side verification.
3.  Run tests using `./caffeine-build/scripts/build.sh unit-tests-gtest`.

## 3. Coding Standards

- **Language:** C11 for application and BSP layers; C++17/20 for tests (GTest).
- **Style:** Allman-style braces, 4-space indentation, 120-column limit.
- **Memory:** No dynamic memory allocation (`malloc`, `free`) in application or BSP layers.
- **Error Handling:** All HAL calls must have their return codes checked using the `cfn_hal_error_code_t` enum.
