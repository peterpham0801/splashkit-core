/**
 * Sound Effect Unit Tests
 */

#include "catch.hpp"

#include "types.h"
#include "audio.h"
#include "resources.h"

using namespace splashkit_lib;

TEST_CASE("sound effects can be loaded, played and freed", "[sound_effect]")
{
    SECTION("can get audio ready")
    {
        open_audio();
        REQUIRE(audio_ready() == true);
    }
    SECTION("can detect non-existent sound effect")
    {
        REQUIRE(has_sound_effect("non_existent") == false);
        sound_effect no_snd = load_sound_effect("non_existent", "non_existent.mp3");
        REQUIRE(no_snd == nullptr);
        REQUIRE(has_sound_effect("non_existent") == false);
    }
    SECTION("can load, play, stop and free sound effect")
    {
        sound_effect snd;
        string filename = "SwinGameStart.wav", name = "SwinGameStart";
        SECTION("can load sound effect")
        {
            REQUIRE(has_sound_effect(name) == false);
            snd = load_sound_effect(name, filename);
            REQUIRE(snd != nullptr);
            REQUIRE(has_sound_effect(name) == true);
            REQUIRE(sound_effect_valid(snd) == true);
            REQUIRE(sound_effect_name(snd) == name);
            REQUIRE(sound_effect_filename(snd) == path_to_resource(filename, SOUND_RESOURCE));
            REQUIRE(sound_effect_named(name) == snd);
        }
        SECTION("can play sound effect")
        {
            REQUIRE(sound_effect_playing(snd) == false);
            play_sound_effect(snd);
            REQUIRE(sound_effect_playing(snd) == true);
        }
        SECTION("can stop sound effect")
        {
            stop_sound_effect(snd);
            REQUIRE(sound_effect_playing(snd) == false);
        }
        SECTION("can free sound effect")
        {
            free_sound_effect(snd);
            REQUIRE(has_sound_effect("error") == false);
        }
    }
    SECTION("can load and free multiple sounds effects")
    {
        sound_effect snd1, snd2;
        string filename1 = "breakdance.wav", name1 = "breakdance", filename2 = "comedy_boing.ogg", name2 = "comedy_boing";
        SECTION("can load and create two sound effects")
        {
            REQUIRE(has_sound_effect(name1) == false);
            snd1 = load_sound_effect(name1, filename1);
            REQUIRE(snd1 != nullptr);
            REQUIRE(has_sound_effect(name1) == true);
            REQUIRE(sound_effect_valid(snd1) == true);
            REQUIRE(sound_effect_name(snd1) == name1);
            REQUIRE(sound_effect_filename(snd1) == path_to_resource(filename1, SOUND_RESOURCE));
            REQUIRE(sound_effect_named(name1) == snd1);

            REQUIRE(has_sound_effect(name2) == false);
            snd2 = load_sound_effect(name2, filename2);
            REQUIRE(snd2 != nullptr);
            REQUIRE(has_sound_effect(name2) == true);
            REQUIRE(sound_effect_valid(snd2) == true);
            REQUIRE(sound_effect_name(snd2) == name2);
            REQUIRE(sound_effect_filename(snd2) == path_to_resource(filename2, SOUND_RESOURCE));
            REQUIRE(sound_effect_named(name2) == snd2);
        }
        SECTION("can free all sound effects")
        {
            free_all_sound_effects();
            REQUIRE(has_sound_effect(name1) == false);
            REQUIRE(has_sound_effect(name2) == false);
        }
    }
    SECTION("handle invalid inputs")
    {
        REQUIRE_THROWS_AS(load_sound_effect("", ""), std::invalid_argument);
        REQUIRE_THROWS_AS(load_sound_effect("", "valid_file.wav"), std::invalid_argument);
        REQUIRE_THROWS_AS(load_sound_effect("valid_name", ""), std::invalid_argument);
    }
    SECTION("handle duplicate sound effect names")
    {
        sound_effect snd1 = load_sound_effect("duplicate_name", "file1.wav");
        REQUIRE(snd1 != nullptr);
        REQUIRE(has_sound_effect("duplicate_name") == true);

        sound_effect snd2 = load_sound_effect("duplicate_name", "file2.wav");
        REQUIRE(snd2 == nullptr); // Assuming duplicate names are not allowed
        REQUIRE(sound_effect_named("duplicate_name") == snd1);
    }
    SECTION("ensure all resources are cleaned up")
    {
        sound_effect snd1 = load_sound_effect("sound1", "file1.wav");
        sound_effect snd2 = load_sound_effect("sound2", "file2.wav");

        free_all_sound_effects();
        REQUIRE(has_sound_effect("sound1") == false);
        REQUIRE(has_sound_effect("sound2") == false);

        // Optionally, check for memory leaks or dangling pointers if applicable
    }
    SECTION("free individual sound effects")
    {
        sound_effect snd1 = load_sound_effect("sound1", "file1.wav");
        sound_effect snd2 = load_sound_effect("sound2", "file2.wav");

        REQUIRE(has_sound_effect("sound1") == true);
        REQUIRE(has_sound_effect("sound2") == true);

        free_sound_effect(snd1);
        REQUIRE(has_sound_effect("sound1") == false);
        REQUIRE(has_sound_effect("sound2") == true);
    }
    SECTION("play and stop edge cases")
    {
        sound_effect snd = load_sound_effect("test_sound", "test.wav");
        REQUIRE(snd != nullptr);

        stop_sound_effect(snd); // Stopping before playing should not cause issues
        REQUIRE(sound_effect_playing(snd) == false);

        play_sound_effect(snd);
        REQUIRE(sound_effect_playing(snd) == true);

        play_sound_effect(snd); // Playing again while already playing should not cause issues
        REQUIRE(sound_effect_playing(snd) == true);

        stop_sound_effect(snd);
        REQUIRE(sound_effect_playing(snd) == false);
    }
}
