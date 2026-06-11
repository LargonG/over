function(set_source_directory r_sources)
	set(m_flag_args "")
	set(m_one_var_args "SOURCE_DIR")
	set(m_multi_var_args "SOURCES")

	cmake_parse_arguments(
		arg
		"${m_flag_args}"
		"${m_one_var_args}"
		"${m_multi_var_args}"
		${ARGN}
	)

	set(${r_sources} "")
	foreach(m_file IN LISTS arg_SOURCES)
		set(m_file "${arg_SOURCE_DIR}/${m_file}")
		list(APPEND ${r_sources} "${m_file}")
	endforeach()

	return(PROPAGATE ${r_sources})
endfunction()
