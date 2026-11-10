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
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.foundation.Canvas
import androidx.compose.ui.geometry.Offset

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        setContent {
            RowingTelemetryTheme {
                Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
                    Greeting(
                        name = "Android",
                        modifier = Modifier.padding(innerPadding)
                    )
                }
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Composable
fun GreetingPreview() {
    RowingTelemetryTheme {
        Greeting("Android")
    }
}


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
@Preview(
    showBackground = true,
    device = "spec:orientation=landscape,width=411dp,height=891dp"
)
@Composable
fun LandscapeMode(modifier: Modifier = Modifier) {
    RowingTelemetryTheme {
        var screen by rememberSaveable() {
            mutableStateOf(NavScreen.HOME)
        }
        when (screen) {
            NavScreen.HOME -> {
                HomeScreen(
                    navigator = { target -> screen = target }
                )
            }
            NavScreen.ROWER -> DataScreen() // TODO
            NavScreen.COACH -> DataScreen() // TODO
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
                    navigator(NavScreen.ROWER)
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

@Preview(
    showBackground = true,
    device = "spec:orientation=landscape,width=411dp,height=891dp"
)
@Composable
fun DataScreen(
    //rowers: List<Int>,
    modifier: Modifier = Modifier
) {
    Row(
        modifier = Modifier.fillMaxSize()
    ) {
        //ForceCurve(rowers)
        ForceCurve(
            modifier = Modifier.weight(1f)
        )
        RowingVerticalDivider()
        Column() {
            SingleValDisplay("Boat Speed", "10")
            RowingHorizontalDivider()
            SingleValDisplay(name = "Distance(m)", data = "2000")
            RowingHorizontalDivider()
            NavPanel()
        }
    }
}

@Composable
fun ForceCurve(
    //rowers: List<Int>,
    modifier: Modifier = Modifier
) {
    Canvas(
        modifier = Modifier
    ) {
        drawLine(
            color = Color.Gray,
            start = Offset(x = 0f, y = size.height / 2),
            end = Offset(x = size.width, y = size.height / 2),
            strokeWidth = 2.dp.toPx()
        )
        drawCircle(
            color = Color.Red,
            radius = 20.dp.toPx(),
            center = Offset(x = size.width / 2, y = size.height / 4)
        )
        // TODO
    }
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
fun NavPanel(modifier: Modifier = Modifier) {
    var paused by rememberSaveable() {
        mutableStateOf(false)
    }
    var togglePause = {
        paused = !paused
    }
    Row(

    ) {
        IconButton(
            onClick = togglePause,
            modifier = Modifier
                .weight(1f)
                .fillMaxHeight()
        ) {
            Icon(
                imageVector = if (paused) Icons.Default.PlayArrow else Icons.Default.Pause,
                contentDescription = if (paused) "Play Button" else "Pause Button",
                modifier = Modifier.fillMaxSize()
            )
        }
        Button(
            onClick = {}, // TODO
            modifier = Modifier
                .weight(1f)
                .fillMaxHeight()
        ) {
            Text(
                text = "Back",
                fontSize = 48.sp
            )
        }
    }
}