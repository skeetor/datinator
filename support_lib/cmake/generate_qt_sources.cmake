
function (generate_qt_sources gen_src_dir gen_bin_dir ui_include_dir generated_sources source_files)

	set(source_list "")

	find_program (QT_UIC uic)
	find_program (QT_MOC moc)
	#message("   File: ${src_files}")

	foreach(src_file ${${source_files}})
		set(gen_file "")
		#message("   File: ${src_file}")
		get_filename_component(src_ext ${src_file} EXT)
		string(TOLOWER ${src_ext} src_ext)

		if( ${src_ext} STREQUAL ".ui" )
			# create include paths for the generated sourcefiles
			get_filename_component(src_path ${src_file} DIRECTORY)
			string(REPLACE "${gen_src_dir}/" "" src_path ${src_path})

			make_directory("${ui_include_dir}/${src_path}")
			string(REPLACE "${gen_src_dir}" "${ui_include_dir}" gen_file ${src_file})
			string(REGEX REPLACE ".ui$" ".h" gen_file "${gen_file}")

			#message("**** ${QT_UIC} -g cpp -o ${gen_file} ${src_file}")
			add_custom_command(
				PRE_BUILD
				OUTPUT "${gen_file}"
				COMMAND ${QT_UIC} -g cpp -o "${gen_file}" "${src_file}"
				MAIN_DEPENDENCY "${src_file}"
				COMMENT "Generating UI: ${gen_file}"
			)
		elseif(${src_ext} STREQUAL ".moc" )

			string(REPLACE "${gen_src_dir}" "${gen_bin_dir}" gen_file ${src_file})
			string(REGEX REPLACE ".moc$" ".cpp" gen_file "${gen_file}")

			#message("   ${QT_MOC} -o ${gen_file} -f ${src_file} ${src_file}")
			add_custom_command(
				PRE_BUILD
				OUTPUT "${gen_file}"
				COMMAND ${QT_MOC} -o "${gen_file}" -f "${src_file}" "${src_file}"
				MAIN_DEPENDENCY "${src_file}"
				COMMENT "Generating MOC: ${gen_file}"
			)
		endif()
	
		#message("FILE: ${src_file}")
		if(NOT ${gen_file} STREQUAL "")
			set(source_list "${source_list};${gen_file}")
			#message("source_list - ${gen_file} ${source_list}")
		endif()
	endforeach()

	set(${generated_sources} "${source_list}" PARENT_SCOPE)
	#message("OUTPUT: ${generated_sources} - ${${generated_sources}}")
	set_source_files_properties(${${generated_sources}} PROPERTIES GENERATED TRUE)

endfunction()
