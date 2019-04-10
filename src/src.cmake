include_directories(${PROJECT_SRC_PATH})

set(SRC_OBJ_PATH ${PROJECT_SRC_PATH}/BotObject)
include_directories(${SRC_OBJ_PATH})
include(${SRC_OBJ_PATH}/object.cmake)

set(SRC_COMMON_PATH ${PROJECT_SRC_PATH}/Common)
include_directories(${SRC_COMMON_PATH})
include(${SRC_COMMON_PATH}/common.cmake)

set(SRC_CTRLER_PATH ${PROJECT_SRC_PATH}/Controller)
include_directories(${SRC_CTRLER_PATH})
include(${SRC_CTRLER_PATH}/controller.cmake)

set(SRC_NET_PATH ${PROJECT_SRC_PATH}/Network)
include_directories(${SRC_NET_PATH})
include(${SRC_NET_PATH}/network.cmake)

set(SRC_VIEW_PATH ${PROJECT_SRC_PATH}/View)
include_directories(${SRC_VIEW_PATH})
include(${SRC_VIEW_PATH}/view.cmake)

set(SRC_FILES
${OBJ_SRC}
${COMMON_SRC}
${CTRLER_SRC}
${NET_SRC}
${VIEW_SRC}
)

set(HEAD_FILES
${OBJ_HEAD}
${COMMON_HEAD}
${CTRLER_HEAD}
${NET_HEAD}
${VIEW_HEAD}

)

set(UI_FILES
${VIEW_UI}
)

set(RESOURCES_FILES
${PROJECT_SRC_PATH}/bot.qrc
)
