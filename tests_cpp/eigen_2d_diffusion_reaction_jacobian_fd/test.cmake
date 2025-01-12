include(FindUnixCommands)

# note that 6,10 is on purpose different to check that in 2d we do things right
set(CMD "python ${MESHDRIVER} -n 6 10 --outDir ${OUTDIR} -s ${STENCILVAL} --bounds 0.0 1.0 0.0 1.0 --periodic false")
execute_process(COMMAND ${BASH} -c ${CMD} RESULT_VARIABLE RES)
if(RES)
  message(FATAL_ERROR "Mesh generation failed")
else()
  message("Mesh generation succeeded!")
endif()

execute_process(COMMAND ${EXENAME} RESULT_VARIABLE CMD_RESULT)
if(RES)
  message(FATAL_ERROR "run failed")
else()
  message("run succeeded!")
endif()
