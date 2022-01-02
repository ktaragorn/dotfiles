import subprocess
import json
import logging

pwd = '/home/ubuntu/'

def report_error(error):
	report_cmd = pwd + 'pushbullet "Error with epic games publisher" ' + '"'+str(error)+ '"'
	subprocess.check_call(report_cmd, shell=True)
	report_cmd = pwd + 'telegram "Error with epic games publisher : ' +str(error)+ '"'
	subprocess.check_call(report_cmd, shell=True)


def report_free_game(gameName):
	report_cmd = pwd + 'pushbullet "Epic Games Free game(s) " ' + '"' + gameName + '"' + ' "-d channel_tag=epicgamesfree"'
	subprocess.check_call(report_cmd, shell=True)
	report_cmd = pwd + 'telegram "Epic Games Free game(s) : '  + gameName + '"'
	subprocess.check_call(report_cmd, shell=True)

currentFreeGameCmd = "curl https://store-site-backend-static.ak.epicgames.com/freeGamesPromotions?locale=en-US | jq '[[.data.Catalog.searchStore.elements[]  | {name: .title,date: .promotions.promotionalOffers[0].promotionalOffers[0].startDate, percent: .promotions.promotionalOffers[0].promotionalOffers[0].discountSetting.discountPercentage}] | .[] |  select(.date != null) | select(.percent == 0) |.name]'"

try:
	# assuming that if multiple games are offered, all are offered and removed at the same time, so treating as one game.
	games = ",".join(json.loads(subprocess.check_output(currentFreeGameCmd, shell=True)))
	try: 
		lastReportedGame = open(pwd + "lastEpicFreeGame").read()
	except FileNotFoundError:
		lastReportedGame = ""
	if(games != lastReportedGame):
		open(pwd + "lastEpicFreeGame", 'w').write(games)
		report_free_game(games)

except Exception as e:
	logging.info('Error ', exc_info=True)
	report_error(e)
