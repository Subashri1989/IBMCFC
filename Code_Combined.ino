wifi.setmode(wifi.STATION)
wifi.sta.config("Subashri","saihari$1")
wifi.eventmon.register(wifi.eventmon.STA_GOT_IP, connected)

function connected (e)
    tmr.alarm(1, 600000, tmr.ALARM_AUTO, function()
        -- Trigger the "dpin_cb” function (defined below)
        -- when a pulse comes in.
        -- dpin_cb will turn off the trigger after 30s
        gpio.trig(dpin, "both", dpin_cb)
    end)
end

function dpin_cb (level, when)
    -- current_level = gpio.read(dpin)
    if prev_level == level then
        -- there is no change. ignore
        return
    else
        prev_level = level
    end
if level == 1 then
        rising_ts = tmr.now()
        print ("raising edge : " .. rising_ts)
        -- turn on the red LED
        gpio.write(lpin, gpio.LOW)
    else
        falling_ts = tmr.now()
        print ("falling edge : " .. falling_ts)
        -- turn off the red LED
        gpio.write(lpin, gpio.HIGH)
    end
    -- Start aggregated timer after a complete pulse is detected
    if falling_ts > 0 and rising_ts > 0 then
        if falling_ts > rising_ts then
            high_time = high_time + falling_ts - rising_ts
        else
            low_time = low_time + rising_ts - falling_ts
        end
    end
    -- Sampling period is 30*1,000,000 macroseconds
    total_time = high_time + low_time
    if total_time > 30000000 then
        lpo = low_time / total_time
        -- remove the INT and reset timers
        gpio.trig(dpin, "none")
        rising_ts = 0
        falling_ts = 0
        high_time = 0
        low_time = 0
        -- turn off the red LED
        gpio.write(lpin, gpio.HIGH)
        -- Very rough estimate. More calibration needed
        pm25 = lpo * 100.0 * 1.5
     
    end
end

m = mqtt.Client(device_id, 120, "use-token-auth", access_token)
m:lwt("dw/lwt", "offline", 0, 0)


m:connect("host.iotp.mqtt.bluemix.com", 1883, 0,
  function(client)
    print("connected")
    -- subscribe topic with QoS = 0
    client:subscribe("dw/air", 0, function(client)
  print("subscribe success")
    end)
    -- publish a message with QoS = 0, retain = 0
    client:publish("dw/air", pm25, 0, 0, function(client)
  print("sent")
    end)
  end,
  function(client, reason)
    print("failed reason: " .. reason)
	
client:subscribe("dw/control", 0)

m:on("message", function(client, topic, data)
  if topic == "dw/control" then
print(data)
t = {}
for k, v in string.gmatch(data, "[^%s]+") do
    t[k] = v
end

if t[0] == device_id then
    -- Change the main loop interval to t[1]
  end
end)

