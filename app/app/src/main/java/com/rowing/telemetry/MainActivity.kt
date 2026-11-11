package com.rowing.telemetry

import android.content.res.Configuration
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.Canvas
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.shape.RoundedCornerShape
import androidx.compose.material3.Button
import androidx.compose.material3.HorizontalDivider
import androidx.compose.material3.Scaffold
import androidx.compose.material3.Text
import androidx.compose.material3.VerticalDivider
import androidx.compose.runtime.Composable
import androidx.compose.runtime.getValue
import androidx.compose.runtime.setValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.saveable.rememberSaveable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.unit.sp
import com.rowing.telemetry.ui.theme.RowingTelemetryTheme
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Pause
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Path
import androidx.compose.ui.graphics.drawscope.DrawScope
import androidx.compose.ui.graphics.drawscope.Stroke
import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.isActive
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asStateFlow
import kotlinx.coroutines.launch
import org.json.JSONObject
import java.net.DatagramPacket
import java.net.DatagramSocket
import androidx.compose.runtime.collectAsState
import java.net.InetAddress
import androidx.lifecycle.viewmodel.compose.viewModel
import android.util.Log
import android.view.WindowManager

class TelemetryViewModel : ViewModel() {
    private val forceCurveData = MutableStateFlow<List<Offset>>(emptyList())
    val readOnlyForceCurveData = forceCurveData.asStateFlow()

    private val distance = MutableStateFlow(0f)
    val readOnlyDistance = distance.asStateFlow()

    private val speed = MutableStateFlow(0f)
    val readOnlySpeed = speed.asStateFlow()

    var paused = MutableStateFlow(false)

    init {
        startListening()
    }

    fun togglePause() {
        paused.value = !paused.value
    }

    private fun startListening() {
        viewModelScope.launch(Dispatchers.IO) {
            try {
                val port = 12345
                val socket = DatagramSocket(port)
                val buffer = ByteArray(1024)

                Log.d("UDP", "Socket opened, waiting for packets...")
                while (isActive) {
                    val packet = DatagramPacket(buffer, buffer.size)
                    socket.receive(packet)

                    if (!paused.value) {
                        val jString = String(packet.data, 0, packet.length)
                        Log.d("UDP", "Received raw packet: $jString")
                        parseData(jString)
                    }
                }
            } catch (e: Exception) {
                e.printStackTrace()
            }
        }
    }

    private fun parseData(jstring: String) {
        try {
            val json = JSONObject(jstring)
            val force = json.getDouble("force").toFloat()
            val angle = json.getDouble("angle").toFloat()
            val dist = json.getDouble("distance").toFloat()
            val spd = json.getDouble("speed").toFloat()
            // TODO

            val newPoint = Offset(angle, force)

            val updatedForceCurveData = forceCurveData.value.toMutableList()

            if (updatedForceCurveData.isNotEmpty() && angle < updatedForceCurveData.last().x) {
                updatedForceCurveData.clear()
            }
            updatedForceCurveData.add(newPoint)

            if (updatedForceCurveData.size > 1000) updatedForceCurveData.removeAt(0)

            forceCurveData.value = updatedForceCurveData
            distance.value = dist
            speed.value = spd
        } catch (e: Exception) {
            Log.e("UDP", "Failed to parse JSON", e)
            e.printStackTrace()
        }
    }
}

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        window.addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON)
        enableEdgeToEdge()
        setContent {
            RowingTelemetryTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    MainApp(
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}

@Preview(
    showBackground = true,
    device = "spec:orientation=landscape,width=411dp,height=891dp"
)
@Composable
fun MainApp(modifier: Modifier = Modifier) {
    val config = LocalConfiguration.current
    val isLandscape = config.orientation == Configuration.ORIENTATION_LANDSCAPE

    if (isLandscape) {
        LandscapeMode()
    } else {
        PortraitMode()
    }
}

@Composable
fun PortraitMode() {
    RowingTelemetryTheme() {
        Box(
            modifier = Modifier.fillMaxSize(),
            contentAlignment = Alignment.Center
        ) {
            Text(
                text = "Please rotate device",
                fontSize = 32.sp
            )
        }
    }
}
enum class NavScreen { HOME, ROWER, COACH }

@Composable
fun LandscapeMode(modifier: Modifier = Modifier) {
    RowingTelemetryTheme {
        var screen by rememberSaveable() {
            mutableStateOf(NavScreen.HOME)
        }

        val view: TelemetryViewModel = viewModel()
        val forceAngleData by view.readOnlyForceCurveData.collectAsState()
        val distance by view.readOnlyDistance.collectAsState()
        val speed by view.readOnlySpeed.collectAsState()

        when (screen) {
            NavScreen.HOME -> {
                HomeScreen(
                    navigator = { target -> screen = target }
                )
            }
            NavScreen.ROWER -> DataScreen(
                forceAngleData = forceAngleData,
                distance = distance,
                speed = speed,
                goToHome = { screen = NavScreen.HOME },
                view = view
            ) // TODO
            NavScreen.COACH -> DataScreen(
                forceAngleData = forceAngleData,
                distance = distance,
                speed = speed,
                goToHome = { screen = NavScreen.HOME },
                view = view
            ) // TODO
        }
    }
}

@Composable
fun HomeScreen(
    navigator: (NavScreen) -> Unit,
    modifier: Modifier = Modifier
) {
    Column(
        modifier = Modifier.fillMaxSize()
    ) {
        Box(
            modifier = Modifier.fillMaxWidth(),
            contentAlignment = Alignment.TopCenter
        ) {
            Text(
                text = "Choose Option",
                fontSize = 48.sp
            )
        }
        Row(
            modifier = Modifier
                .padding(horizontal = 24.dp, vertical = 20.dp)
                .fillMaxWidth(),
            verticalAlignment = Alignment.CenterVertically,
            horizontalArrangement = Arrangement.Center
        ) {
            Button(
                modifier = Modifier
                    .weight(1f)
                    .fillMaxHeight(),
                onClick = {
                    navigator(NavScreen.ROWER)
                },
                shape = RoundedCornerShape(48.dp)
            ) {
                Text(
                    text = "I am a rower",
                    fontSize = 48.sp
                    )
            }
            RowingVerticalDivider()
            Button(
                modifier = Modifier
                    .weight(1f)
                    .fillMaxHeight(),
                onClick = {
                    navigator(NavScreen.COACH)
                },
                shape = RoundedCornerShape(48.dp)
            ) {
                Text(
                    text = "I am a coach",
                    fontSize = 48.sp
                    )
            }
        }
    }
}

@Composable
fun DataScreen(
    modifier: Modifier = Modifier,
    forceAngleData: List<Offset>,
    distance: Float,
    speed: Float,
    goToHome: () -> Unit = {},
    view: TelemetryViewModel,
) {
    Row(
        modifier = Modifier.fillMaxSize()
    ) {
        //ForceCurve(rowers)
        ForceCurve(
            forceAngleData = forceAngleData,
            modifier = Modifier.weight(1f)
        )
        RowingVerticalDivider()
        Column(
            modifier = Modifier.weight(1f)
        ) {
            SingleValDisplay("Boat Speed", speed.toString())
            RowingHorizontalDivider()
            SingleValDisplay(name = "Distance(m)", data = distance.toString())
            RowingHorizontalDivider()
            NavPanel(goToHome = goToHome, view = view)
        }
    }
}

@Composable
fun ForceCurve(
    forceAngleData: List<Offset>,
    modifier: Modifier = Modifier
) {
    val path = remember { Path() }
    Canvas(
        modifier = modifier.fillMaxSize()
    ) {
        val canvasWidth = size.width
        val canvasHeight = size.height

        val strokeWidth = 2.dp.toPx()
        val adjustment = canvasHeight / 10f

        // x-axis
        drawLine(
            color = Color.Gray,
            start = Offset(adjustment, canvasHeight - adjustment),
            end = Offset(canvasWidth - adjustment, canvasHeight - adjustment),
            strokeWidth = strokeWidth
        )

        // y-axis
        drawLine(
            color = Color.Gray,
            start = Offset(x = adjustment, y = adjustment),
            end = Offset(x = adjustment, y = canvasHeight - adjustment),
            strokeWidth = strokeWidth
        )

        // if (telemetryData.size < 2) return@Canvas
        updateSingleCurve(
            data = forceAngleData,
            path = path,
            canvasWidth = canvasWidth,
            canvasHeight = canvasHeight,
            fixedMaxX = 1000f,
            fixedMaxY = 1000f,
            adjustment = adjustment
        )
    }
}

fun DrawScope.updateSingleCurve(
    data: List<Offset> = emptyList(),
    path: Path,
    color: Color = Color.Blue,
    canvasWidth: Float = 100f,
    canvasHeight: Float = 100f,
    fixedMaxX: Float = 1000f,
    fixedMaxY: Float = 1000f,
    adjustment: Float
) {
    path.reset()
    if (data.size < 2) return

    val graphWidth = canvasWidth - (adjustment * 2)
    val graphHeight = canvasHeight - (adjustment * 2)

    data.forEachIndexed { index, point ->
        val x = adjustment + (point.x / fixedMaxX) * graphWidth
        val y = (canvasHeight - adjustment) - ((point.y / fixedMaxY) * graphHeight)

        if (index == 0) {
            path.moveTo(x, y)
        } else {
            path.lineTo(x, y)
        }
    }

    drawPath(
        path = path,
        color = color,
        style = Stroke(width = 3.dp.toPx())
    )
}

@Composable
fun RowingHorizontalDivider(modifier: Modifier = Modifier) {
    HorizontalDivider(
        modifier = Modifier
            .fillMaxWidth()
            .padding(16.dp),
        thickness = 2.dp,
        color = Color.Gray
    )
}

@Composable
fun RowingVerticalDivider(modifier: Modifier = Modifier) {
    VerticalDivider(
        modifier = Modifier
            .fillMaxHeight()
            .padding(16.dp),
        thickness = 2.dp,
        color = Color.Gray
    )
}

@Composable
fun SingleValDisplay(
    name: String,
    data: String,
    modifier: Modifier = Modifier
) {
    Column(
        modifier = Modifier.fillMaxWidth()
    ) {
        Text(
            text = name,
            fontSize = 20.sp
        )
        Text(
            text = data,
            fontSize = 36.sp,
            modifier = Modifier.align(Alignment.CenterHorizontally)
        )
    }
}

@Composable
fun NavPanel(
    modifier: Modifier = Modifier,
    goToHome: () -> Unit,
    view: TelemetryViewModel
) {
    val isPaused by view.paused.collectAsState()
    Row(
        modifier = Modifier.padding(horizontal = 24.dp, vertical = 20.dp)
    ) {
        IconButton(
            onClick = { view.togglePause() },
            modifier = Modifier
                .weight(1f)
                .fillMaxHeight(),
            shape = RoundedCornerShape(48.dp)
        ) {
            Icon(
                imageVector = if (isPaused) Icons.Default.PlayArrow else Icons.Default.Pause,
                contentDescription = if (isPaused) "Play Button" else "Pause Button",
                modifier = Modifier.fillMaxSize()
            )
        }
        RowingVerticalDivider()
        Button(
            onClick = goToHome,
            modifier = Modifier
                .weight(1f)
                .fillMaxHeight(),
            shape = RoundedCornerShape(48.dp)
        ) {
            Text(
                text = "Back",
                fontSize = 48.sp
            )
        }
    }
}

fun zeroAngle() {
    val port = 12345 // TODO
    val ip = "192.168.4.1"

    val message = "T"
    val data = message.toByteArray()
    val packet = DatagramPacket(
        data,
        data.size,
        InetAddress.getByName(ip),
        port
    )

    val sock = DatagramSocket()
    sock.send(packet)
    sock.close()
}