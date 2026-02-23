
function(init_submodule SUBMODULE_PATH)
	if (NOT EXISTS "${CMAKE_SOURCE_DIR}/${SUBMODULE_PATH}")
		message(STATUS "Fetching submodule ${SUBMODULE_PATH}...")

		find_package(Git REQUIRED)

		execute_process(
			COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${SUBMODULE_PATH}
			WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
			RESULT_VARIABLE GIT_RESULT
		)

		if (NOT GIT_RESULT EQUAL 0)
			message(FATAL_ERROR "Failed to fetch submodule ${SUBMODULE_PATH}")
		endif()
	else()
		message(STATUS "Submodule already exists: ${SUBMODULE_PATH}")
	endif()
endfunction()
