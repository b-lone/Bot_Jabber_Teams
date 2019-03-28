include_directories(${PROJECT_SRC_PATH})

set(SRC_OBJ_PATH ${PROJECT_SRC_PATH}/BotObject)
include_directories(${SRC_OBJ_PATH})
include(${SRC_OBJ_PATH}/object.cmake)

set(SRC_FILES
${PROJECT_SRC_PATH}/main.cpp
${PROJECT_SRC_PATH}/botmainwindow.cpp
${PROJECT_SRC_PATH}/botconfig.cpp
${PROJECT_SRC_PATH}/botnetworkmanager.cpp
${PROJECT_SRC_PATH}/botserver.cpp
${OBJ_SRC}
)

set(HEAD_FILES
${PROJECT_SRC_PATH}/botcommon.h
${PROJECT_SRC_PATH}/botmainwindow.h
${PROJECT_SRC_PATH}/botconfig.h
${PROJECT_SRC_PATH}/botnetworkmanager.h
${PROJECT_SRC_PATH}/botserver.h
${OBJ_HEAD}
)

set(UI_FILES
${PROJECT_SRC_PATH}/botmainwindow.ui
)

set(RESOURCES_FILES
${PROJECT_SRC_PATH}/bot.qrc
)
