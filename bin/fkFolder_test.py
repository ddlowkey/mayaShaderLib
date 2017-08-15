import pymel.core as pm
import time


def clear():
    for i in range(0, 10):
        for h in range(0, 10):
            pm.headsUpDisplay(rp=(i, h))


def frameNumber(*args):
    frame = pm.currentTime(q=1)
    return "%04d" % frame


def localTime(*args):
    return time.asctime(time.localtime(time.time()))


def filePath(*args):
    return pm.sceneName()


def timeCount(t):
    t = t + 0.0416
    return "%04d" % t


def setRefresh():
    if not pm.objExists('headsup'):
        pm.mel.eval(
            'expression -s "headsUpDisplay -r currentFrame; headsUpDisplay -r currentTime; headsUpDisplay -r currentRate;" -n "headsup"; refresh;')
    else:
        pm.delete('headsup')
        pm.mel.eval(
            'expression -s "headsUpDisplay -r currentFrame; headsUpDisplay -r currentTime; headsUpDisplay -r currentRate;" -n "headsup"; refresh;')


clear()

pm.headsUpDisplay('currentFrame', section=8, block=1, blockSize='small', labelFontSize='large', label='Frame',
                  command=frameNumber, event='timeChanged', dfs='large')
pm.headsUpDisplay('currentTime', section=3, block=1, blockSize='medium', labelFontSize='large', command=localTime,
                  event='timeChanged', dfs='large')
pm.headsUpDisplay('currentPath', section=2, block=1, blockSize='medium', labelFontSize='large', command=filePath,
                  event='timeChanged', dfs='large')
pm.headsUpDisplay('currentRate', section=8, block=0, blockSize='medium', labelFontSize='large', dfs='large',
                  pre='frameRate')

setRefresh()



