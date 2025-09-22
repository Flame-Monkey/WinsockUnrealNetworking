# 프로토콜
HTTP/2에서 따옴

# 메시지 매니저, 메모리 풀
메시지 종류별 크기 구분 없이 고정된 크기로 메모리 풀 할당

(ChattingMessage(151 bytes), FriendMessage(51 bytes) => 구분 없이 192bytes 할당)

대용량 파일 송수신 같은 큰 메시지의 경우, 별도의 메모리 풀 할당(슬롯당 640 bytes)