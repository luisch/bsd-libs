# mint normal installer

MACRO(LOCAL_INSTALL TARGET_NAME)
	INSTALL(TARGETS ${TARGET_NAME}
		RUNTIME DESTINATION bin
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
	#
	# Debug用のpdbファイルをインストール先にコピーする。
	# だが、msvc2013以降ではpdbの出来る位置が変わってしまった？ため
	# 対応できない。あと、cmake 3.0.0以降でLOCATIONを使うと警告が出るので
	# ( $<LOCATION:Debug> で取りに行くようになってるのかな？)
	# このアプローチはダメになっているっぽい。
	#
	#IF(MSVC)
	#
	#	IF(CMAKE_VERSION VERSION_GREATER "2.9.5")
	#	 CMAKE_POLICY(SET CMP0026 OLD)
	#	 CMAKE_POLICY(SET CMP0045 OLD)
	#	ENDIF()
	#
	#	FOREACH( BCONFIG
	#		DEBUG
	#		RELWITHDEBINFO
	#	)
	#		GET_TARGET_PROPERTY(OUT_PATH ${TARGET_NAME} LOCATION_${BCONFIG})
	#		IF( ${OUT_PATH} MATCHES "${CMAKE_STATIC_LIBRARY_SUFFIX}$" )
	#			STRING(REGEX REPLACE "\\.([a-zA-Z0-9_]+)$" ".pdb" OUT_PATH ${OUT_PATH})
	#			INSTALL( FILES
	#				${OUT_PATH}
	#				DESTINATION lib
	#				CONFIGURATIONS ${BCONFIG}
	#			)
	#		ENDIF()
	#	ENDFOREACH()
	#ENDIF()

ENDMACRO()
