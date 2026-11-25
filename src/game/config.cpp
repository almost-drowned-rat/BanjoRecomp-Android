#include <algorithm>
#include "banjo_config.h"
#include "composites/ui_mod_discovery.h"
#include "ultramodern/ultramodern.hpp"
#include "recompui/recompui.h"
#include "recompui/config.h"
#include "recompinput/recompinput.h"
#include "banjo_sound.h"
#include "banjo_support.h"
#include "ultramodern/config.hpp"
#include "librecomp/files.hpp"
#include "librecomp/config.hpp"
#include "util/file.h"
#include <filesystem>
#include <fstream>
#include <iomanip>

#if defined(_WIN32)
#include <Shlobj.h>
#elif defined(__linux__)
#include <unistd.h>
#include <pwd.h>
#elif defined(__APPLE__)
#include "apple/rt64_apple.h"
#endif

static void add_general_options(recomp::config::Config &config) {
    using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;

    static EnumOptionVector note_saving_mode_options = {
        {banjo::NoteSavingMode::Off,        "Off",    "Off"},
        {banjo::NoteSavingMode::OnlyNotes,  "Notes",  "Only Notes"},
        {banjo::NoteSavingMode::OnlyJinjos, "Jinjos", "Only Jinjos"},
        // Keeping as "On" to preserve compatibility with previous configs before jinjo saving
        {banjo::NoteSavingMode::Both,       "On",     "Both"},
    };
    config.add_enum_option(
        banjo::configkeys::general::note_saving_mode,
        "Note & Jinjo Saving",
        "Saves collected notes and jinjos so that you don't need to collect them again when revisiting a level. <recomp-color primary>Both</recomp-color> is the default, while <recomp-color primary>off</recomp-color> matches the original game.<br /><br />Changing this setting will only take effect when entering/exiting a level, or restarting the game.",
        note_saving_mode_options,
        banjo::NoteSavingMode::Both
    );
    static EnumOptionVector analog_cam_mode_options = {
        {banjo::AnalogCamMode::Off, "Off", "Off"},
        {banjo::AnalogCamMode::On, "On", "On"},
    };
    config.add_enum_option(
        banjo::configkeys::general::analog_cam_mode,
        "Analog Camera",
        "Enables the analog camera.",
        analog_cam_mode_options,
        banjo::AnalogCamMode::Off
    );
    config.add_number_option(
        banjo::configkeys::general::analog_camera_sensitivity,
        "Analog Camera Sensitivity",
        "Sets the sensitivity of the right stick analog camera, if enabled.",
        1, 10, 1, 0, false, 3
    );
    config.add_option_hidden_dependency(
        banjo::configkeys::general::analog_camera_sensitivity,
        banjo::configkeys::general::analog_cam_mode,
        banjo::AnalogCamMode::Off
    );
    static EnumOptionVector camera_invert_mode_options = {
        {banjo::CameraInvertMode::InvertNone, "InvertNone", "None"},
        {banjo::CameraInvertMode::InvertX, "InvertX", "Invert X"},
        {banjo::CameraInvertMode::InvertY, "InvertY", "Invert Y"},
        {banjo::CameraInvertMode::InvertBoth, "InvertBoth", "Invert Both"}
    };
    config.add_enum_option(
        banjo::configkeys::general::third_person_camera_invert_mode,
        "Invert Camera",
        "Inverts the camera controls for the third person camera if it's enabled. <recomp-color primary>Invert X</recomp-color> is the default and matches the original game.<br /><br />If analog camera is off, only the <recomp-color primary>Invert X</recomp-color> setting will take effect.",
        camera_invert_mode_options,
        banjo::CameraInvertMode::InvertX
    );
    static EnumOptionVector first_person_invert_mode_options = {
        {banjo::CameraInvertMode::InvertNone, "InvertNone", "None"},
        {banjo::CameraInvertMode::InvertX, "InvertX", "Invert X"},
        {banjo::CameraInvertMode::InvertY, "InvertY", "Invert Y"},
        {banjo::CameraInvertMode::InvertBoth, "InvertBoth", "Invert Both"}
    };
    config.add_enum_option(
        banjo::configkeys::general::first_person_invert_mode,
        "Invert First Person View",
        "Inverts the camera controls in first person view. <recomp-color primary>Invert Y</recomp-color> is the default and matches the original game.",
        first_person_invert_mode_options,
        banjo::CameraInvertMode::InvertY
    );
    static EnumOptionVector flying_and_swimming_invert_options = {
        {banjo::CameraInvertMode::InvertNone, "InvertNone", "None"},
        {banjo::CameraInvertMode::InvertX, "InvertX", "Invert X"},
        {banjo::CameraInvertMode::InvertY, "InvertY", "Invert Y"},
        {banjo::CameraInvertMode::InvertBoth, "InvertBoth", "Invert Both"}
    };
    config.add_enum_option(
        banjo::configkeys::general::flying_and_swimming_invert_mode,
        "Invert Flying & Swimming",
        "Inverts the controls for swimming and flying. <recomp-color primary>Invert Y</recomp-color> is the default and matches the original game.",
        flying_and_swimming_invert_options,
        banjo::CameraInvertMode::InvertY
    );
}

template <typename T = uint32_t>
T get_general_config_enum_value(const std::string& option_id) {
    return static_cast<T>(std::get<uint32_t>(recompui::config::get_general_config().get_option_value(option_id)));
}

template <typename T = uint32_t>
T get_general_config_number_value(const std::string& option_id) {
    return static_cast<T>(std::get<double>(recompui::config::get_general_config().get_option_value(option_id)));
}

banjo::NoteSavingMode banjo::get_note_saving_mode() {
    return get_general_config_enum_value<banjo::NoteSavingMode>(banjo::configkeys::general::note_saving_mode);
}

banjo::CameraInvertMode banjo::get_camera_invert_mode() {
    return get_general_config_enum_value<banjo::CameraInvertMode>(banjo::configkeys::general::camera_invert_mode);
}

banjo::CameraInvertMode banjo::get_third_person_camera_mode() {
    return get_general_config_enum_value<banjo::CameraInvertMode>(banjo::configkeys::general::third_person_camera_invert_mode);
}

banjo::CameraInvertMode banjo::get_flying_and_swimming_invert_mode() {
    return get_general_config_enum_value<banjo::CameraInvertMode>(banjo::configkeys::general::flying_and_swimming_invert_mode);
}

banjo::CameraInvertMode banjo::get_first_person_invert_mode() {
    return get_general_config_enum_value<banjo::CameraInvertMode>(banjo::configkeys::general::first_person_invert_mode);
}

banjo::AnalogCamMode banjo::get_analog_cam_mode() {
    return get_general_config_enum_value<banjo::AnalogCamMode>(banjo::configkeys::general::analog_cam_mode);
}

uint32_t banjo::get_analog_cam_sensitivity() {
    return get_general_config_number_value(banjo::configkeys::general::analog_camera_sensitivity);
}

template <typename T = uint32_t>
T get_graphics_config_enum_value(const std::string& option_id) {
    return static_cast<T>(std::get<uint32_t>(recompui::config::get_graphics_config().get_option_value(option_id)));
}

static void add_sound_options(recomp::config::Config &config) {
    config.add_percent_number_option(
        banjo::configkeys::sound::bgm_volume,
        "Background Music Volume",
        "Controls the overall volume of background music.",
        100.0f
    );
}
template <typename T = uint32_t>
T get_sound_config_number_value(const std::string& option_id) {
    return static_cast<T>(std::get<double>(recompui::config::get_sound_config().get_option_value(option_id)));
}

int banjo::get_bgm_volume() {
    return get_sound_config_number_value<int>(banjo::configkeys::sound::bgm_volume);
}

static void add_graphics_options(recomp::config::Config &config) {
    using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;
    static EnumOptionVector cutscene_aspect_ratio_mode_options = {
        {banjo::CutsceneAspectRatioMode::Original, "Original", "Original"},
        {banjo::CutsceneAspectRatioMode::Clamp16x9, "Clamp16x9", "16:9"},
        {banjo::CutsceneAspectRatioMode::Full, "Expand", "Expand"},
    };
    config.add_enum_option(
        banjo::configkeys::graphics::cutscene_aspect_ratio_mode,
        "Cutscene Aspect Ratio",
        "Sets the aspect ratio limit for cutscenes. Cutscenes have been adjusted to work in <recomp-color primary>16:9</recomp-color>, which is the default option. Wider aspect ratios may show details that weren't meant to be on-screen.",
        cutscene_aspect_ratio_mode_options,
        banjo::CutsceneAspectRatioMode::Clamp16x9
    );    
}

static void set_control_defaults() {
    using namespace recompinput;

    // Left shoulder -> C Down | Backwards eggs / zoom out
    set_default_mapping_for_controller(
        GameInput::C_DOWN,
        { 
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY, true),
            InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER)
        }
    );

    // Right shoulder -> C Up | Forwards eggs / first person
    set_default_mapping_for_controller(
        GameInput::C_UP,
        { 
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTY, false),
            InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)
        }
    );

    // North button -> C Left | Talon trot / camera left
    set_default_mapping_for_controller(
        GameInput::C_LEFT,
        { 
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX, false),
            InputField::controller_digital(SDL_CONTROLLER_BUTTON_NORTH)
        }
    );

    // East button -> C Right | Wonderwing / camera right
    set_default_mapping_for_controller(
        GameInput::C_RIGHT,
        { 
            InputField::controller_analog(SDL_GameControllerAxis::SDL_CONTROLLER_AXIS_RIGHTX, true),
            InputField::controller_digital(SDL_CONTROLLER_BUTTON_EAST)
        }
    );

    // R3 -> L | Unused in BK but can be used in mods
    set_default_mapping_for_controller(GameInput::L, { InputField::controller_digital(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSTICK) });
}

static void set_control_descriptions() {
    recompinput::set_game_input_description(recompinput::GameInput::Y_AXIS_POS, "Used to move and for steering while flying and swimming. Axis inversion for flying and swimming can be configured in the General tab.");
    recompinput::set_game_input_description(recompinput::GameInput::Y_AXIS_NEG, "Used to move and for steering while flying and swimming. Axis inversion for flying and swimming can be configured in the General tab.");
    recompinput::set_game_input_description(recompinput::GameInput::X_AXIS_NEG, "Used to move and for steering while flying and swimming. Axis inversion for flying and swimming can be configured in the General tab.");
    recompinput::set_game_input_description(recompinput::GameInput::X_AXIS_POS, "Used to move and for steering while flying and swimming. Axis inversion for flying and swimming can be configured in the General tab.");
    recompinput::set_game_input_description(recompinput::GameInput::A, "Used to jump and select options in menus. Also used for flying upwards.");
    recompinput::set_game_input_description(recompinput::GameInput::B, "Used for attacks, which change depending on whether you are stationary, moving, in the air, or crouching.");
    recompinput::set_game_input_description(recompinput::GameInput::Z, "Used to crouch, which enables A, B and the C-Buttons to perform different actions.");
    recompinput::set_game_input_description(recompinput::GameInput::L, "Used to skip dialogue boxes during game replays (press L + R + B simultaneously). Mods may also use it separately for additional features.");
    recompinput::set_game_input_description(recompinput::GameInput::R, "Used to center the camera behind Banjo on the ground, and to perform tighter turns while flying or swimming.");
    recompinput::set_game_input_description(recompinput::GameInput::START, "Used for pausing and for skipping certain cutscenes.");
    recompinput::set_game_input_description(recompinput::GameInput::C_UP, "Used to enter first-person mode, and to shoot eggs while holding Z.");
    recompinput::set_game_input_description(recompinput::GameInput::C_DOWN, "Used to toggle between the different camera zoom levels, and to shoot eggs backwards while holding Z.");
    recompinput::set_game_input_description(recompinput::GameInput::C_LEFT, "Used to rotate the camera sideways. Axis inversion can be configured in the General tab. Also used to enter Talon Trot while holding Z.");
    recompinput::set_game_input_description(recompinput::GameInput::C_RIGHT, "Used to rotate the camera sideways. Axis inversion can be configured in the General tab). Also used to enter Wonderwing while holding Z.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_UP, "Unused. Mods may use it for additional features.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_DOWN, "Unused. Mods may use it for additional features.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_LEFT, "Unused. Mods may use it for additional features.");
    recompinput::set_game_input_description(recompinput::GameInput::DPAD_RIGHT, "Unused. Mods may use it for additional features.");

}

banjo::CutsceneAspectRatioMode banjo::get_cutscene_aspect_ratio_mode() {
    return get_graphics_config_enum_value<banjo::CutsceneAspectRatioMode>(banjo::configkeys::graphics::cutscene_aspect_ratio_mode);
}

#ifdef __ANDROID__
// The Android card menu (the same in all N64 ports: General, the game's own areas, Graphics, Shaders, Audio,
// Controls, Mods). The configs stay as they are (saved settings keep their files); their own tabs are hidden.
recompui::config::MenuCardEntry android_driver_card();   // src/main/main.cpp

static void create_android_card_tabs() {
    using namespace recompui::config;
    namespace gk = banjo::configkeys::general;
    set_java_menu_class("io.github.almostdrownedrat.banjo.TouchControls");
    set_menu_accent(31, 99, 194);   // the start screen's colour (the blue sky)
    create_card_tab("General", "card_general", []() {
        std::vector<MenuCardEntry> l;
        l.push_back(header_card("Session"));
        MenuCardEntry back;
        back.label = "Back to game";
        back.run = []() { recompui::config::close(); };
        l.push_back(back);
        for (auto &e : java_menu_cards("game")) l.push_back(e);                 // Exit game
        l.push_back(header_card("Gameplay"));
        l.push_back(option_card(general::id, gk::note_saving_mode));
        l.push_back(header_card("Input"));
        l.push_back(option_card(general::id, general::options::background_input_mode));
        return l;
    });
    create_card_tab("Camera", "card_camera", []() {
        return std::vector<MenuCardEntry>{
            header_card("Camera"),
            option_card(general::id, gk::analog_cam_mode),
            option_card(general::id, gk::analog_camera_sensitivity),
            header_card("Invert"),
            option_card(general::id, gk::third_person_camera_invert_mode),
            option_card(general::id, gk::first_person_invert_mode),
            option_card(general::id, gk::flying_and_swimming_invert_mode),
        };
    });
    create_card_tab("Graphics", "card_graphics", []() {
        namespace go = graphics::options;
        std::vector<MenuCardEntry> l;
        std::vector<std::string> used;
        auto add = [&](const std::string &id) {
            l.push_back(option_card(graphics::id, id));
            used.push_back(id);
        };
        l.push_back(header_card("Picture"));
        add(go::res_option);
        add(go::ar_option);
        add(go::hr_option);
        add(banjo::configkeys::graphics::cutscene_aspect_ratio_mode);
        l.push_back(header_card("Quality"));
        add(go::ds_option);
        add(go::msaa_option);
        // any other option of the graphics config (shown only where it isn't hidden)
        auto schema = get_config(graphics::id).get_config_schema();
        for (auto &o : schema.options) {
            if (o.id != go::rr_option && o.id != go::rr_manual_value &&
                std::find(used.begin(), used.end(), o.id) == used.end()) {
                add(o.id);
            }
        }
        l.push_back(header_card("Frame rate"));
        add(go::rr_option);
        add(go::rr_manual_value);
        for (auto &e : java_menu_cards("display")) l.push_back(e);              // FPS
        l.push_back(header_card("Driver"));
        l.push_back(android_driver_card());
        return l;
    }, graphics::id);
    create_card_tab("Shaders", "card_shaders", []() {
        std::vector<MenuCardEntry> l;
        l.push_back(header_card("RetroArch shaders"));
        for (auto &e : java_menu_cards("shader")) l.push_back(e);
        l.push_back(header_card("Picture"));
        MenuCardEntry res = option_card("android_options", "shader_resolution");
        res.hidden = nullptr;                                                     // hidden as an option of its own tab
        l.push_back(res);
        return l;
    });
    create_card_tab("Audio", "card_sound", []() {
        std::vector<MenuCardEntry> l;
        l.push_back(header_card("Volume"));
        for (auto &e : config_cards(sound::id)) l.push_back(e);
        return l;
    });
    create_card_tab("Controls", "card_controls", []() {
        std::vector<MenuCardEntry> l;
        l.push_back(header_card("Controller"));
        l.push_back(bindings_card());
        l.push_back(option_card(general::id, general::options::joystick_deadzone));
        l.push_back(option_card(general::id, general::options::rumble_strength));
        l.push_back(header_card("Touch controls"));
        for (auto &e : java_menu_cards("touch")) l.push_back(e);
        return l;
    });
}
#endif

void banjo::init_config() {
    std::filesystem::path recomp_dir = recompui::file::get_app_folder_path();

    if (!recomp_dir.empty()) {
        std::filesystem::create_directories(recomp_dir);
    }

    recompui::config::GeneralTabOptions general_options{};
    general_options.has_rumble_strength = true;
    general_options.has_gyro_sensitivity = false;
    general_options.has_mouse_sensitivity = false;

    auto &general_config = recompui::config::create_general_tab(general_options);
    add_general_options(general_config);

    auto &graphics_config = recompui::config::create_graphics_tab();
    add_graphics_options(graphics_config);

    set_control_defaults();
    set_control_descriptions();
#ifdef __ANDROID__
    // Settings > Android Options (after Graphics): the shader resolution; main.cpp adds the buttons for the shader
    // pages of the menu Back opens and the GPU driver.
    {
        auto &android_config = recompui::config::create_config_tab("Android Options", "android_options", false);
        using EnumOptionVector = const std::vector<recomp::config::ConfigOptionEnumOption>;
        static EnumOptionVector shader_resolution_options = {
            {0, "Original", "Original"},
            {1, "2x", "2x"},
            {2, "Full", "Full"}
        };
        android_config.add_enum_option(
            "shader_resolution",
            "Shader Resolution",
            "The size of the game's picture a RetroArch shader works on. "
            "<recomp-color primary>Original</recomp-color>: the N64's 240 lines, as in RetroArch - the look the shaders "
            "were made for (scanlines, CRT masks, pixel upscalers) and by far the lightest. "
            "<recomp-color primary>2x</recomp-color>: twice that. "
            "<recomp-color primary>Full</recomp-color>: the window's size - heavy shaders get slow.<br /><br />"
            "<b>Shader...</b> imports shader packs (.zip) and picks a preset, <b>Shader parameters...</b> changes "
            "the preset's settings, <b>GPU Driver (Turnip)...</b> installs a Vulkan driver (the app restarts).",
            shader_resolution_options,
            0u,
            true   // shown as a button (src/main/main.cpp)
        );
    }
#endif
    recompui::config::create_controls_tab();

    auto &sound_config = recompui::config::create_sound_tab();
    add_sound_options(sound_config);

#ifdef __ANDROID__
    for (const std::string &id : { recompui::config::general::id, recompui::config::graphics::id, std::string("android_options"),
                                   recompui::config::controls::id, recompui::config::sound::id }) {
        recompui::config::set_tab_visible(id, false);
    }
    create_android_card_tabs();
#endif
    recompui::config::create_mods_tab();
#ifdef __ANDROID__
    recompui::config::set_tab_visible(recompui::config::mods::id, false);
    if (!recompui::get_discovery_url().empty() && !ultramodern::is_game_started()) {
        recompui::config::set_tab_visible(recompui::config::mod_discovery::id, false);
    }
    recompui::config::create_card_tab("Mods", "card_mods", []() {
        std::vector<recompui::config::MenuCardEntry> l;
        l.push_back(recompui::config::header_card("Mods"));
        for (auto &e : recompui::config::mods_cards()) l.push_back(e);
        return l;
    });
#endif

    recompui::config::finalize();

}

