include_guard(GLOBAL)

if (USE_VCPKG)
	find_package(imgui CONFIG REQUIRED)
else()
	include(InitSubmodules)

	init_submodule(libs/imgui)

	# suffer
	set(m_IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)

	add_library(imgui STATIC
		${m_IMGUI_DIR}/imgui.cpp
		${m_IMGUI_DIR}/imgui_draw.cpp
		${m_IMGUI_DIR}/imgui_tables.cpp
		${m_IMGUI_DIR}/imgui_widgets.cpp
		${m_IMGUI_DIR}/backends/imgui_impl_glfw.cpp
		${m_IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
		${m_IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
	)

	target_link_libraries(imgui PRIVATE glfw)
	target_include_directories(imgui PUBLIC ${m_IMGUI_DIR}/ ${m_IMGUI_DIR}/backends/ ${m_IMGUI_DIR}/misc/cpp)

	add_library(imgui::imgui ALIAS imgui)
endif()
