include_directories(${PROJECT_SRC_PATH})

set(SRC_OBJ_PATH ${PROJECT_SRC_PATH}/BotObject)
include_directories(${SRC_OBJ_PATH})
include(${SRC_OBJ_PATH}/object.cmake)

set(SRC_CTRLER_PATH ${PROJECT_SRC_PATH}/Controller)
include_directories(${SRC_CTRLER_PATH})
include(${SRC_CTRLER_PATH}/controller.cmake)

set(SRC_FILES
${OBJ_SRC}
${CTRLER_SRC}
${PROJECT_SRC_PATH}/main.cpp
${PROJECT_SRC_PATH}/botmainwindow.cpp
${PROJECT_SRC_PATH}/botconfig.cpp
${PROJECT_SRC_PATH}/botnetworkmanager.cpp
${PROJECT_SRC_PATH}/botserver.cpp
${PROJECT_SRC_PATH}/botnetworkcontroller.cpp
${PROJECT_SRC_PATH}/botcommon.cpp
${PROJECT_SRC_PATH}/botprocesshelper.cpp
)

set(HEAD_FILES
${OBJ_HEAD}
${CTRLOR_HEAD}
${PROJECT_SRC_PATH}/botcommon.h
${PROJECT_SRC_PATH}/botmainwindow.h
${PROJECT_SRC_PATH}/botconfig.h
${PROJECT_SRC_PATH}/botnetworkmanager.h
${PROJECT_SRC_PATH}/botserver.h
${PROJECT_SRC_PATH}/botnetworkcontroller.h
${PROJECT_SRC_PATH}/botprocesshelper.h
)

set(UI_FILES
${PROJECT_SRC_PATH}/botmainwindow.ui
)

set(RESOURCES_FILES
${PROJECT_SRC_PATH}/bot.qrc
)
