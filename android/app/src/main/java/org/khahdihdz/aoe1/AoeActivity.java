package org.khahdihdz.aoe1;
import org.libsdl.app.SDLActivity;
/** SDL2 host activity; rendering and simulation run in native C++. */
public final class AoeActivity extends SDLActivity {
  @Override protected String[] getLibraries() { return new String[] { "SDL2", "main" }; }
}
