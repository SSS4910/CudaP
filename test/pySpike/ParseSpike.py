#Spike for Parsing

import string
import sys
'''class hostData:
        host = ''
        number = 0;
        def __init__(self, inhost):
            self.host = inhost
        setHost(inhost):
            self.host = inhost
        getHost():
            return host
        inc():
            self.number = self.number + 1'''


uniHost = []
uniHost2 = []
def listHost():
    global uniHost
    for x in uniHost:
        print x
    #print uniHost.len()



def analize(host , rfc, auth, time, command, code,  size, referer, userAgent):
    global uniHost
    global uniHost2

    #print host + ' ' + time + ' ' + command+ ' %d' % code + ' %d' % size 
    flag = 0
    #print host
    '''for x in uniHost:
        if x == host:
            flag == 1
    '''
    flag = uniHost.count(host)
    if flag == 0: 
        uniHost.append(host)
    
    '''flag = 0
    for x in uniHost2:
        if x.getHost == x:
            flag = 1
            break
    if flag == 0:'''


    '''if command[1:4] != "GET":
        print command'''


def parse_line(line):
    #print line
    linedata = list(line)
    n = 0;
    i = 0;
    host = ''
    rfc = ''
    auth = ''
    time = ''
    command = ''
    retCode = ''
    code = 0
    size = 0
    retSize = ''
    referer = '' 
    userAgent = ''
    flag = 0;
    banana = 0;

    for x in linedata: 
        if n == 0:
            if x == ' ':
                n = 1
            else:
                host = host + x
        elif n == 1:
            if x == ' ':
                n = 2
            else:
                rfc = rfc + x
        elif n == 2:
            if x == ' ':
                n = 3
            else:
                auth = auth + x
        elif n == 3:
            if flag == 1:
                n = 4
                flag = 0
            elif x == ']':
                flag = 1
                px = '&'
                time = time + x
            else:
                time = time + x
        elif n == 4:
            if flag == 2:
                '''if x != ' ' or linedata[i-2] == "/":
                    if linedata[i+1] == '0':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '1':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '2':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '3':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '4':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '5':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '6':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '7':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '8':
                        flag = 0
                        n = 5
                    elif linedata[i+1] == '9':
                        flag = 0
                        n = 5
                    else:
                        n = 4
                        flag = 1
                        command = command + x '''
                if x != ' ':
                    flag =  1
                    n = 4
                    banana = 1
                    command = command + x    
                else:
                    flag = 0
                    n = 5
            elif x == '"':
                command = command + x
                flag = flag + 1
            else:
                command = command + x
        elif n == 5:
            if x == ' ':
                n = 6
                try:
                    code = int(retCode)
                except ValueError:
                    banana = 1
                    command = command + retCode + x
                    retCode = ''
                    n = 5
            else: 
                retCode = retCode + x
        elif n == 6:
            if x == ' ':
                n = 7
                size = long(retSize)
            elif x == '-':
                retSize = retSize + '0'
            else:
                retSize = retSize + x

        elif n == 7:
            if flag == 2:
                flag = 0
                n = 8
            elif x == '"':
                referer = referer + x
                flag = flag + 1
            else:
                referer = referer + x
        elif n == 8:
            if flag == 2:
                flag = 0
                n = 9
            elif x == '"':
                userAgent = userAgent + x
                flag = flag + 1
            else:
                userAgent = userAgent + x
        i = i + 1

    if banana == 1:
        print ' ' + retCode + ' ' + command
    #print '##' + retCode + '##' + retSize + command
    #code = int(retCode)
    #size = long(retSize)
            

    #print host + ' ' + time + ' ' + command+ ' %d' % code + ' %d' % size 
        # code is all generated
    analize(host , rfc, auth, time, command, code,  size, referer, userAgent)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print 'WRONG'
    else:
        j = 0;
        f = open(sys.argv[1], 'r')
        for l in f:
            parse_line(l)
            #print j
            j = j + 1
        

        #listHost()
        print 'done :D'
        f.close()

