function(compile_shaders)
	set(m_flag_args "")
	set(m_one_var_args "SOURCE_DIR;DESTINATION_DIR")
	set(m_multi_var_args "SOURCES")

	cmake_parse_arguments(
		arg
		"${m_flag_args}"
		"${m_one_var_args}"
		"${m_multi_var_args}"
		${ARGN}
	)

	foreach(m_file IN LISTS arg_SOURCES)
		configure_file(
			"${arg_SOURCE_DIR}/${m_file}" "${arg_DESTINATION_DIR}/${m_file}" COPYONLY
		)
	endforeach()

endfunction()
