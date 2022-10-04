add_requires("sfml")
add_requires("imgui-sfml")

target("Fractal_sfml")
  set_kind("binary")
  set_languages("cxx20")

  add_files("src/*.cpp")
  add_headerfiles("shaders/**.glsl")
	add_installfiles("shaders/**.glsl", {prefixdir = "bin/shaders"})
  add_headerfiles("src/*.hpp")

  add_headerfiles("palette/*.png")
  add_installfiles("palette/*.png", {prefixdir = "bin/palette"})

  set_rundir(".")

  add_packages("sfml")
  add_packages("imgui-sfml")

