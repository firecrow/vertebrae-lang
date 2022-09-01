(closure
    (#def 
        :y "yellow"
        :x '(#def
            :alpha "apple" 
            :bravo "beat" 
            :charlie "carrot")
        :r "red")
        (print y)
        (print x)
        (print "after things"))
