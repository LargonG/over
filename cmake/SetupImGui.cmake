include_guard(GLOBAL)

if (USE_VCPKG)
	find_package(imgui CONFIG REQUIRED)
else()
	include(InitSubmodules)

	init_submodule(libs/imgui)

	# suffer
	set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/libs/imgui)

	add_library(imgui STATIC
		${IMGUI_DIR}/imgui.cpp
		${IMGUI_DIR}/imgui_draw.cpp
		${IMGUI_DIR}/imgui_tables.cpp
		${IMGUI_DIR}/imgui_widgets.cpp
		${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
		${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
		${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
	)

	target_link_libraries(imgui PRIVATE glfw)
	target_include_directories(imgui PUBLIC ${IMGUI_DIR}/ ${IMGUI_DIR}/backends/ ${IMGUI_DIR}/misc/cpp)

	add_library(imgui::imgui ALIAS imgui)
endif()
