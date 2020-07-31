def bridge_disconnected(topic, message):
	return message == "2"

def bridge_connected(topic, message):
	return message == "1"