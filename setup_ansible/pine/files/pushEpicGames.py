import subprocess
import json
import logging

pwd = '/home/ubuntu/'

def report_error(error):
	report_cmd = pwd + 'pushbullet "Error with epic games publisher" ' + '"'+str(error)+ '"'
	subprocess.check_call(report_cmd, shell=True)

def report_free_game(gameName):
	report_cmd = pwd + 'pushbullet "Epic Games Free game" ' + '"' + gameName + '"' + ' "-d channel_tag=epicgamesfree"'
	subprocess.check_call(report_cmd, shell=True)

currentFreeGameCmd = "curl https://store-site-backend-static.ak.epicgames.com/freeGamesPromotions?locale=en-US | jq '[.data.Catalog.searchStore.elements[]  | {name: .title,date: .promotions.promotionalOffers[0].promotionalOffers[0].startDate}] | .[] |  select(.date != null)'"

try:
	gameData = json.loads(subprocess.check_output(currentFreeGameCmd, shell=True))
	lastReportedGame = open(pwd + "lastEpicFreeGame").read()
	if(gameData['name'] != lastReportedGame):
		open(pwd + "lastEpicFreeGame", 'w').write(gameData['name'])
		report_free_game(gameData['name'])

except Exception as e:
	logging.info('Error ', exc_info=True)
	report_error(e)
