include_guard(GLOBAL)

if (USE_VCPKG)
	find_package(imgui CONFIG REQUIRED)
else()
	include(InitSubmodules)

	init_submodule(libs/imgui)

	# suffer

	set(m_imgui_dir ${CMAKE_SOURCE_DIR}/libs/imgui)

	set(m_sources
		imgui.cpp
		imgui_draw.cpp
		imgui_tables.cpp
		imgui_widgets.cpp
		
		backends/imgui_impl_glfw.cpp
		backends/imgui_impl_opengl3.cpp
		misc/cpp/imgui_stdlib.cpp
	)
	list(TRANSFORM m_sources PREPEND ${m_imgui_dir}/)

	set(m_root_interface
		imgui.h
	)
	list(TRANSFORM m_root_interface PREPEND ${m_imgui_dir}/)

	set(m_misc_cpp_interface
		imgui_stdlib.h
	)
	list(TRANSFORM m_misc_cpp_interface PREPEND ${m_imgui_dir}/misc/cpp/)

	set(m_impl_interface
		imgui_impl_glfw.h
		imgui_impl_opengl3.h
	)
	list(TRANSFORM m_impl_interface PREPEND ${m_imgui_dir}/backends/)

	add_library(imgui STATIC EXCLUDE_FROM_ALL)
	target_sources(imgui
		PRIVATE ${m_sources}
		PUBLIC FILE_SET m_root_interface TYPE HEADERS BASE_DIRS ${m_imgui_dir} FILES ${m_root_interface}
		PUBLIC FILE_SET stdlib TYPE HEADERS BASE_DIRS ${m_imgui_dir}/misc/cpp FILES ${m_misc_cpp_interface}
		PUBLIC FILE_SET impl TYPE HEADERS BASE_DIRS ${m_imgui_dir}/backends FILES ${m_impl_interface}
	)
	target_link_libraries(imgui PRIVATE glfw)

	add_library(imgui::imgui ALIAS imgui)
endif()
