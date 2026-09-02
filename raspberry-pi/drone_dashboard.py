from flask import Flask, request, jsonify
import serial
import threading


app = Flask(__name__)


# UART settings

UART_PORT = "/dev/serial0"
UART_BAUD = 115200

uart_lock = threading.Lock()


def send_to_stm32(message):

    try:

        with uart_lock:

            ser = serial.Serial(
                UART_PORT,
                UART_BAUD,
                timeout=0.1
            )

            ser.write(message.encode())

            ser.flush()

            ser.close()

        return True


    except serial.SerialException as error:

        print("UART ERROR:", error)

        return False



@app.route("/")
def home():

    return """
    <!DOCTYPE html>

    <html lang="en">

    <head>

        <meta charset="UTF-8">

        <meta
            name="viewport"
            content="width=device-width, initial-scale=1.0"
        >

        <title>Drone Dashboard</title>


        <style>

            * {
                box-sizing: border-box;
            }


            body {

                background-color: #111827;

                color: white;

                font-family: Arial, sans-serif;

                text-align: center;

                margin: 0;

                padding: 20px;
            }


            h1 {

                margin-bottom: 30px;
            }


            .controller {

                display: flex;

                justify-content: center;

                gap: 60px;

                flex-wrap: wrap;
            }


            .stick-container {

                text-align: center;
            }


            .joystick {

                width: 240px;

                height: 240px;

                background-color: #1f2937;

                border: 3px solid #374151;

                border-radius: 50%;

                position: relative;

                margin: auto;

                touch-action: none;

                user-select: none;
            }


            .stick {

                width: 70px;

                height: 70px;

                background-color: #2563eb;

                border-radius: 50%;

                position: absolute;

                left: 50%;

                top: 50%;

                transform: translate(-50%, -50%);

                cursor: grab;
            }


            .stick:active {

                background-color: #1d4ed8;

                cursor: grabbing;
            }


            .top-label {

                position: absolute;

                top: 10px;

                left: 50%;

                transform: translateX(-50%);

                color: #9ca3af;

                font-size: 12px;
            }


            .bottom-label {

                position: absolute;

                bottom: 10px;

                left: 50%;

                transform: translateX(-50%);

                color: #9ca3af;

                font-size: 12px;
            }


            .left-label {

                position: absolute;

                left: 10px;

                top: 50%;

                transform: translateY(-50%);

                color: #9ca3af;

                font-size: 12px;
            }


            .right-label {

                position: absolute;

                right: 10px;

                top: 50%;

                transform: translateY(-50%);

                color: #9ca3af;

                font-size: 12px;
            }


            .values {

                margin-top: 15px;

                line-height: 1.8;
            }


            button {

                color: white;

                border: none;

                border-radius: 12px;

                padding: 14px 22px;

                margin: 6px;

                font-size: 16px;

                cursor: pointer;
            }


            button:active {

                transform: scale(0.96);
            }


            .arm {

                background-color: #16a34a;
            }


            .disarm {

                background-color: #d97706;
            }


            .land {

                background-color: #7c3aed;
            }


            .emergency {

                background-color: #dc2626;
            }


            .status-panel {

                margin: 30px auto;

                background-color: #1f2937;

                width: min(450px, 100%);

                padding: 20px;

                border-radius: 15px;

                text-align: left;
            }


            .status-panel h2 {

                text-align: center;

                margin-top: 0;
            }


            .status-panel p {

                margin: 12px 0;
            }


            .value {

                float: right;

                font-weight: bold;
            }


            .armed {

                color: #22c55e;
            }


            .disarmed {

                color: #ef4444;
            }


            .connected {

                color: #22c55e;
            }


            .error {

                color: #ef4444;
            }

        </style>

    </head>



    <body>


        <h1>Drone Dashboard</h1>



        <div class="controller">


            <!-- LEFT STICK -->


            <div class="stick-container">


                <h2>Throttle / Yaw</h2>


                <div
                    class="joystick"
                    id="leftJoystick"
                >


                    <span class="top-label">
                        Throttle +
                    </span>


                    <span class="bottom-label">
                        Throttle -
                    </span>


                    <span class="left-label">
                        Yaw Left
                    </span>


                    <span class="right-label">
                        Yaw Right
                    </span>


                    <div
                        class="stick"
                        id="leftStick">
                    </div>


                </div>



                <div class="values">


                    Throttle:

                    <span id="throttleValue">
                        0
                    </span>%


                    <br>


                    Yaw:

                    <span id="yawValue">
                        0
                    </span>%


                </div>


            </div>



            <!-- RIGHT STICK -->


            <div class="stick-container">


                <h2>Pitch / Roll</h2>


                <div
                    class="joystick"
                    id="rightJoystick"
                >


                    <span class="top-label">
                        Forward
                    </span>


                    <span class="bottom-label">
                        Backward
                    </span>


                    <span class="left-label">
                        Left
                    </span>


                    <span class="right-label">
                        Right
                    </span>


                    <div
                        class="stick"
                        id="rightStick">
                    </div>


                </div>



                <div class="values">


                    Pitch:

                    <span id="pitchValue">
                        0
                    </span>%


                    <br>


                    Roll:

                    <span id="rollValue">
                        0
                    </span>%


                </div>


            </div>


        </div>



        <!-- DRONE COMMANDS -->


        <div style="margin-top: 30px;">


            <button
                class="arm"
                onclick="armDrone()">

                ARM

            </button>


            <button
                class="disarm"
                onclick="disarmDrone()">

                DISARM

            </button>


            <button
                class="land"
                onclick="landDrone()">

                LAND

            </button>


            <button
                class="emergency"
                onclick="emergencyStop()">

                EMERGENCY STOP

            </button>


        </div>



        <!-- TELEMETRY -->


        <div class="status-panel">


            <h2>Telemetry</h2>


            <p>

                Drone Status:

                <span
                    id="droneStatus"
                    class="value disarmed">

                    DISARMED

                </span>

            </p>


            <p>

                Throttle:

                <span
                    id="telemetryThrottle"
                    class="value">

                    0%

                </span>

            </p>


            <p>

                Pitch:

                <span
                    id="telemetryPitch"
                    class="value">

                    0%

                </span>

            </p>


            <p>

                Roll:

                <span
                    id="telemetryRoll"
                    class="value">

                    0%

                </span>

            </p>


            <p>

                Yaw:

                <span
                    id="telemetryYaw"
                    class="value">

                    0%

                </span>

            </p>


            <p>

                Altitude:

                <span
                    id="altitude"
                    class="value">

                    -- m

                </span>

            </p>


            <p>

                Battery:

                <span
                    id="battery"
                    class="value">

                    -- %

                </span>

            </p>


            <p>

                UART:

                <span
                    id="uartStatus"
                    class="value">

                    --

                </span>

            </p>


        </div>



        <script>


            let armed = false;


            let throttle = 0;

            let yaw = 0;

            let pitch = 0;

            let roll = 0;


            let lastSendTime = 0;



            const leftJoystick =
                document.getElementById(
                    "leftJoystick"
                );


            const leftStick =
                document.getElementById(
                    "leftStick"
                );


            const rightJoystick =
                document.getElementById(
                    "rightJoystick"
                );


            const rightStick =
                document.getElementById(
                    "rightStick"
                );


            let leftDragging = false;

            let rightDragging = false;



            // --------------------------------------------------
            // SEND CONTROL PACKET
            // --------------------------------------------------


            async function sendControl()
            {

                /*
                 * Limit UART updates to about
                 * 20 packets per second.
                 */

                const now = Date.now();


                if (now - lastSendTime < 50)
                {
                    return;
                }


                lastSendTime = now;



                const packet = {

                    throttle: throttle,

                    pitch: pitch,

                    roll: roll,

                    yaw: yaw
                };


                try
                {

                    const response =
                        await fetch(
                            "/control",
                            {
                                method: "POST",

                                headers:
                                {
                                    "Content-Type":
                                        "application/json"
                                },

                                body:
                                    JSON.stringify(
                                        packet
                                    )
                            }
                        );


                    const data =
                        await response.json();


                    if (data.success)
                    {

                        document.getElementById(
                            "uartStatus"
                        ).innerText =
                            "CONNECTED";


                        document.getElementById(
                            "uartStatus"
                        ).className =
                            "value connected";
                    }

                    else
                    {

                        document.getElementById(
                            "uartStatus"
                        ).innerText =
                            "ERROR";


                        document.getElementById(
                            "uartStatus"
                        ).className =
                            "value error";
                    }


                }

                catch (error)
                {

                    document.getElementById(
                        "uartStatus"
                    ).innerText =
                        "ERROR";


                    console.error(error);
                }

            }



            // --------------------------------------------------
            // SEND SPECIAL COMMAND
            // --------------------------------------------------


            async function sendCommand(command)
            {

                try
                {

                    const response =
                        await fetch(
                            "/command",
                            {
                                method: "POST",

                                headers:
                                {
                                    "Content-Type":
                                        "application/json"
                                },

                                body:
                                    JSON.stringify(
                                        {
                                            command:
                                                command
                                        }
                                    )
                            }
                        );


                    const data =
                        await response.json();


                    if (data.success)
                    {

                        document.getElementById(
                            "uartStatus"
                        ).innerText =
                            "CONNECTED";


                        document.getElementById(
                            "uartStatus"
                        ).className =
                            "value connected";
                    }

                    else
                    {

                        document.getElementById(
                            "uartStatus"
                        ).innerText =
                            "ERROR";


                        document.getElementById(
                            "uartStatus"
                        ).className =
                            "value error";
                    }


                }

                catch (error)
                {

                    document.getElementById(
                        "uartStatus"
                    ).innerText =
                        "ERROR";


                    console.error(error);
                }

            }



            // --------------------------------------------------
            // LEFT STICK
            // --------------------------------------------------


            leftStick.addEventListener(
                "pointerdown",
                function(event)
                {

                    leftDragging = true;


                    leftStick.setPointerCapture(
                        event.pointerId
                    );
                }
            );



            leftStick.addEventListener(
                "pointermove",
                function(event)
                {

                    if (!leftDragging)
                    {
                        return;
                    }


                    moveLeftStick(event);

                    sendControl();
                }
            );



            leftStick.addEventListener(
                "pointerup",
                function()
                {

                    leftDragging = false;


                    /*
                     * Throttle stays where
                     * it was released.
                     *
                     * Yaw returns to zero.
                     */


                    yaw = 0;


                    document.getElementById(
                        "yawValue"
                    ).innerText = 0;


                    document.getElementById(
                        "telemetryYaw"
                    ).innerText = "0%";


                    updateLeftStick();


                    sendControl();
                }
            );



            function moveLeftStick(event)
            {

                const rect =
                    leftJoystick
                    .getBoundingClientRect();


                const centerX =
                    rect.width / 2;


                const centerY =
                    rect.height / 2;


                const maxDistance =
                    rect.width / 2 - 35;



                let x =
                    event.clientX
                    - rect.left
                    - centerX;


                let y =
                    event.clientY
                    - rect.top
                    - centerY;



                x = Math.max(
                    -maxDistance,
                    Math.min(
                        maxDistance,
                        x
                    )
                );


                y = Math.max(
                    -maxDistance,
                    Math.min(
                        maxDistance,
                        y
                    )
                );



                yaw = Math.round(
                    (x / maxDistance)
                    * 100
                );



                throttle = Math.round(
                    (
                        (
                            maxDistance - y
                        )
                        /
                        (
                            2 * maxDistance
                        )
                    )
                    * 100
                );



                throttle =
                    Math.max(
                        0,
                        Math.min(
                            100,
                            throttle
                        )
                    );



                document.getElementById(
                    "yawValue"
                ).innerText =
                    yaw;



                document.getElementById(
                    "throttleValue"
                ).innerText =
                    throttle;



                document.getElementById(
                    "telemetryThrottle"
                ).innerText =
                    throttle + "%";



                document.getElementById(
                    "telemetryYaw"
                ).innerText =
                    yaw + "%";



                leftStick.style.left =
                    centerX
                    + x
                    + "px";



                leftStick.style.top =
                    centerY
                    + y
                    + "px";
            }



            function updateLeftStick()
            {

                const rect =
                    leftJoystick
                    .getBoundingClientRect();


                const maxDistance =
                    rect.width / 2 - 35;



                const y =
                    maxDistance
                    -
                    (
                        throttle / 100
                    )
                    *
                    (
                        2
                        * maxDistance
                    );



                leftStick.style.left =
                    "50%";


                leftStick.style.top =
                    (
                        rect.height / 2
                        + y
                    )
                    + "px";
            }



            // --------------------------------------------------
            // RIGHT STICK
            // --------------------------------------------------


            rightStick.addEventListener(
                "pointerdown",
                function(event)
                {

                    rightDragging = true;


                    rightStick.setPointerCapture(
                        event.pointerId
                    );
                }
            );



            rightStick.addEventListener(
                "pointermove",
                function(event)
                {

                    if (!rightDragging)
                    {
                        return;
                    }


                    moveRightStick(event);

                    sendControl();
                }
            );



            rightStick.addEventListener(
                "pointerup",
                function()
                {

                    rightDragging = false;


                    /*
                     * Pitch and roll both
                     * return to zero.
                     */


                    pitch = 0;

                    roll = 0;



                    document.getElementById(
                        "pitchValue"
                    ).innerText = 0;


                    document.getElementById(
                        "rollValue"
                    ).innerText = 0;


                    document.getElementById(
                        "telemetryPitch"
                    ).innerText = "0%";


                    document.getElementById(
                        "telemetryRoll"
                    ).innerText = "0%";



                    rightStick.style.left =
                        "50%";


                    rightStick.style.top =
                        "50%";


                    sendControl();
                }
            );



            function moveRightStick(event)
            {

                const rect =
                    rightJoystick
                    .getBoundingClientRect();


                const centerX =
                    rect.width / 2;


                const centerY =
                    rect.height / 2;


                const maxDistance =
                    rect.width / 2 - 35;



                let x =
                    event.clientX
                    - rect.left
                    - centerX;


                let y =
                    event.clientY
                    - rect.top
                    - centerY;



                x = Math.max(
                    -maxDistance,
                    Math.min(
                        maxDistance,
                        x
                    )
                );


                y = Math.max(
                    -maxDistance,
                    Math.min(
                        maxDistance,
                        y
                    )
                );



                roll = Math.round(
                    (
                        x
                        / maxDistance
                    )
                    * 100
                );



                pitch = Math.round(
                    (
                        -y
                        / maxDistance
                    )
                    * 100
                );



                document.getElementById(
                    "pitchValue"
                ).innerText =
                    pitch;



                document.getElementById(
                    "rollValue"
                ).innerText =
                    roll;



                document.getElementById(
                    "telemetryPitch"
                ).innerText =
                    pitch + "%";



                document.getElementById(
                    "telemetryRoll"
                ).innerText =
                    roll + "%";



                rightStick.style.left =
                    centerX
                    + x
                    + "px";


                rightStick.style.top =
                    centerY
                    + y
                    + "px";
            }



            // --------------------------------------------------
            // ARM
            // --------------------------------------------------


            function armDrone()
            {

                armed = true;


                sendCommand("ARM");


                const status =
                    document.getElementById(
                        "droneStatus"
                    );


                status.innerText =
                    "ARMED";


                status.classList.remove(
                    "disarmed"
                );


                status.classList.add(
                    "armed"
                );
            }



            // --------------------------------------------------
            // DISARM
            // --------------------------------------------------


            function disarmDrone()
            {

                armed = false;


                throttle = 0;

                pitch = 0;

                roll = 0;

                yaw = 0;


                resetSticks();


                sendCommand("DISARM");


                const status =
                    document.getElementById(
                        "droneStatus"
                    );


                status.innerText =
                    "DISARMED";


                status.classList.remove(
                    "armed"
                );


                status.classList.add(
                    "disarmed"
                );
            }



            // --------------------------------------------------
            // LAND
            // --------------------------------------------------


            function landDrone()
            {

                if (!armed)
                {
                    return;
                }


                sendCommand("LAND");


                document.getElementById(
                    "droneStatus"
                ).innerText =
                    "LANDING";
            }



            // --------------------------------------------------
            // EMERGENCY STOP
            // --------------------------------------------------


            function emergencyStop()
            {

                armed = false;


                throttle = 0;

                pitch = 0;

                roll = 0;

                yaw = 0;


                resetSticks();


                sendCommand("ESTOP");


                const status =
                    document.getElementById(
                        "droneStatus"
                    );


                status.innerText =
                    "EMERGENCY STOP";


                status.classList.remove(
                    "armed"
                );


                status.classList.add(
                    "disarmed"
                );
            }



            // --------------------------------------------------
            // RESET STICKS
            // --------------------------------------------------


            function resetSticks()
            {

                document.getElementById(
                    "throttleValue"
                ).innerText = 0;


                document.getElementById(
                    "yawValue"
                ).innerText = 0;


                document.getElementById(
                    "pitchValue"
                ).innerText = 0;


                document.getElementById(
                    "rollValue"
                ).innerText = 0;



                document.getElementById(
                    "telemetryThrottle"
                ).innerText = "0%";


                document.getElementById(
                    "telemetryYaw"
                ).innerText = "0%";


                document.getElementById(
                    "telemetryPitch"
                ).innerText = "0%";


                document.getElementById(
                    "telemetryRoll"
                ).innerText = "0%";



                updateLeftStick();


                rightStick.style.left =
                    "50%";


                rightStick.style.top =
                    "50%";
            }



            // --------------------------------------------------
            // PAGE START
            // --------------------------------------------------


            window.onload = function()
            {

                throttle = 0;

                pitch = 0;

                roll = 0;

                yaw = 0;


                updateLeftStick();

            };


        </script>


    </body>


    </html>
    """


# ------------------------------------------------------------
# RECEIVE JOYSTICK VALUES FROM WEBSITE
# ------------------------------------------------------------


@app.route("/control", methods=["POST"])
def control():


    data = request.get_json(
        silent=True
    )


    if not data:

        return jsonify(
            {
                "success": False
            }
        )


    throttle = int(
        data.get(
            "throttle",
            0
        )
    )


    pitch = int(
        data.get(
            "pitch",
            0
        )
    )


    roll = int(
        data.get(
            "roll",
            0
        )
    )


    yaw = int(
        data.get(
            "yaw",
            0
        )
    )



    # Limit values


    throttle = max(
        0,
        min(
            100,
            throttle
        )
    )


    pitch = max(
        -100,
        min(
            100,
            pitch
        )
    )


    roll = max(
        -100,
        min(
            100,
            roll
        )
    )


    yaw = max(
        -100,
        min(
            100,
            yaw
        )
    )



    # Build STM32 packet


    message = (

        f"T:{throttle} "

        f"P:{pitch} "

        f"R:{roll} "

        f"Y:{yaw}\\n"

    )



    print(
        "CONTROL:",
        message.strip()
    )



    success =
        send_to_stm32(
            message
        )



    return jsonify(
        {
            "success":
                success
        }
    )



# ------------------------------------------------------------
# RECEIVE SPECIAL COMMAND FROM WEBSITE
# ------------------------------------------------------------


@app.route("/command", methods=["POST"])
def command():


    data = request.get_json(
        silent=True
    )


    if not data:

        return jsonify(
            {
                "success": False
            }
        )



    command =
        str(
            data.get(
                "command",
                ""
            )
        ).upper()



    valid_commands =
        [
            "ARM",
            "DISARM",
            "LAND",
            "ESTOP"
        ]



    if command not in valid_commands:

        return jsonify(
            {
                "success": False
            }
        )



    message =
        f"CMD:{command}\\n"



    print(
        "COMMAND:",
        message.strip()
    )



    success =
        send_to_stm32(
            message
        )



    return jsonify(
        {
            "success":
                success
        }
    )



if __name__ == "__main__":


    app.run(

        host="0.0.0.0",

        port=5001,

        debug=False
    )