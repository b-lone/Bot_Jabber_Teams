
#--------------FilesCopy
function(FilesCopy frompath destination)
    #message("FilesCopy:${path},${destination}")

    file(GLOB files "${frompath}/*.dll")
    #message("${files}")

    foreach(loop_value ${files})
        #message(${loop_value})
        file(COPY ${loop_value} DESTINATION ${destination})
    endforeach(loop_value)

endfunction(FilesCopy)

#--------------FilesCopyToPaths
function(FilesCopyToPaths frompath)
    FilesCopy(${frompath} "${PROJECT_OUTPUT_PARH}/Debug")
    FilesCopy(${frompath} "${PROJECT_OUTPUT_PARH}/Release")
    FilesCopy(${frompath} "${PROJECT_OUTPUT_PARH}/Wrap")
endfunction(FilesCopyToPaths)

#--------------FileCopyToPaths
function(FileCopyToPaths file)

    file(COPY ${file} DESTINATION "${PROJECT_OUTPUT_PARH}/Debug")
    file(COPY ${file} DESTINATION "${PROJECT_OUTPUT_PARH}/Release")
    file(COPY ${file} DESTINATION "${PROJECT_OUTPUT_PARH}/Wrap")

endfunction(FileCopyToPaths)
