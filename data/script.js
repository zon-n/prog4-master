function getRandom(min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

setInterval(() => {
  const now = (new Date()).getTime();

  // Les randoms numbers
  const throttle = getRandom(0, 100);
  const steering = getRandom(-45, 45); // degrés
  const wifi = getRandom(-90, -30); // dBm

  document.getElementById('throttleDisplay').textContent = `Throttle: ${throttle}`;
  document.getElementById('steeringDisplay').textContent = `Steering: ${steering}`;
  document.getElementById('wifiDisplay').textContent = `WiFi: ${wifi} dBm`;

  // Les points sur chaque graphique
  chartT.series[0].addPoint([now, throttle], true, chartT.series[0].data.length > 20);
  chart2.series[0].addPoint([now, wifi], true, chart2.series[0].data.length > 20);

  // Fleche de direction
  const arrow = document.querySelector('.arrow');
  arrow.style.transform = `translateX(-50%) rotate(${steering}deg)`;

}, 500); // chaque demi-seconde