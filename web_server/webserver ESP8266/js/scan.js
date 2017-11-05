function startScanAp() {
	let rs = fetch('/APScan.json');
	rs.then(res=>{
		res.json().then(data=>{
			console.log(data);
			if(data){
				console.log("START APScanResults");
				let rs1 = fetch('/APScanResults.json');
				rs1.then(res1=>{
					res1.json().then(data1=>{
						let arData = data1.scan;
						console.log(arData);
						let i = 0, htmlTable = "";
						
						for(let data of arData){
							i++;
							htmlTable += `
								<tr>
									<td><p>${i}</p></td>
									<td><p>${data.essid}</p></td>
									<td><p>${data.bssid}</p></td>
									<td><p>${data.channel}</p></td>
									<td><p>${data.rssi}</p></td>
									<td><p>${data.encrypt}</p></td>
									<td><p>${data.hidden}</p></td>
									<td>
										<button class="btn-main" id="selectId${i}" >Select</button>
									</td>
								</tr>
							`;
						}
						document.querySelector('#tabel-scan-contain table thead').innerHTML = `
							<tr><th>STT</th><th>ESSID</th><th>BSSID</th><th>CHANEL</th><th>RSSI</th><th>ENCRYPT</th><th>HIDEN</th><th>SELECT</th></tr>
						`;
						document.querySelector('#tabel-scan-contain table tbody').innerHTML = htmlTable;
					})
				})
			}
		})
	})		
}