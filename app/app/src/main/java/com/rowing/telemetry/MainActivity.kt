package com.rowing.telemetry

import android.content.res.Configuration
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.gestures.snapping.SnapPosition
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
            NavScreen.ROWER -> RowerScreen()
            NavScreen.COACH -> CoachScreen()
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
                fontSize = 32.sp
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
            VerticalDivider(
                modifier = Modifier
                    .fillMaxHeight()
                    .padding(16.dp),
                thickness = 2.dp,
                color = Color.Gray
            )
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

@Composable
fun RowerScreen(modifier: Modifier = Modifier) {
    Text("rower screen")
}

@Composable
fun CoachScreen(modifier: Modifier = Modifier) {

}

@Composable
fun ForceCurve(modifier: Modifier = Modifier) {
    
}