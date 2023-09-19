set(sources
    src/crypto/stegofile.cpp
    src/crypto/xorencryption.cpp
    src/simpletlsconnection.cpp
    src/telegram/message.cpp
    src/telegram/tgbot.cpp
)

set(exe_sources
		src/main.cpp
		${sources}
)

set(headers
    include/telegram/message.h
    include/telegram/tgbot.h
    include/crypto/stegocontainer.h
    include/crypto/xorencryption.h
    include/simpletlsconnection.h
)

set(test_sources
  src/main.cpp
  src/xor_test.cpp
  src/stegocontainer_test.cpp
  src/telegram_message_test.cpp
)
