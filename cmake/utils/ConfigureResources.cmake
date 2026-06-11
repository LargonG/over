function(set_resources)

# for internal usage only
macro(set_resources_debug_mode)
	file(CREATE_LINK "${CMAKE_CURRENT_SOURCE_DIR}/${m_DIRECTORY}" "${CMAKE_CURRENT_BINARY_DIR}/${m_DIRECTORY}" SYMBOLIC)
endmacro()

# for internal usage only
macro(set_resource_release_mode)
	file(COPY "${m_DIRECTORY}" DESTINATION "/")
endmacro()

	set(m_flag_args "DEBUG;RELEASE")
	set(m_one_var_args "DIRECTORY")
	set(m_multi_var_args "")
	
	cmake_parse_arguments(
		m
		"${m_flag_args}"
		"${m_one_var_args}"
		"${m_multi_var_args}"
		${ARGN}
	)

	if(m_DEBUG AND m_RELEASE)
		message(FATAL_ERROR "DEBUG & RELEASE at the same time are not allowed")
	elseif(NOT m_DEBUG AND NOT m_RELEASE)
		message(FATAL_ERROR "DEBUG|RELEASE flag required")
	endif()

	if(m_DEBUG)
		set_resources_debug_mode()
	else()
		set_resource_release_mode()
	endif()
	

endfunction()
