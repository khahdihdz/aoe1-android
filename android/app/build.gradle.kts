plugins { id("com.android.application") }
android {
  namespace = "org.khahdihdz.aoe1"
  compileSdk = 35
  defaultConfig {
    applicationId = "org.khahdihdz.aoe1"
    minSdk = 23
    targetSdk = 35
    versionCode = 1
    versionName = "0.1.0-foundation"
    ndk { abiFilters += listOf("arm64-v8a") }
    externalNativeBuild {
      cmake {
        cppFlags += listOf("-std=c++20", "-Wall", "-Wextra")
        arguments += listOf("-DANDROID_STL=c++_shared")
      }
    }
  }
  externalNativeBuild {
    cmake { path = file("../../CMakeLists.txt"); version = "3.22.1" }
  }
  buildTypes {
    debug { applicationIdSuffix = ".debug"; versionNameSuffix = "-debug" }
    release { isMinifyEnabled = false }
  }
  packaging { jniLibs { useLegacyPackaging = true } }
}
