(function(scan) {
  //Private Property
  var numSelectST = 0;

  //Public Method

  scan.Attack = (de_dis, isAll)=>{
  	Attack(de_dis, isAll);
  }

  scan.startScanAp = ()=>{
  	startScanAp();
  }
  scan.selectAP = (i)=>{
  	selectAP(i);
  }
  scan.startScanST = ()=>{
  	console.log("Start SCAN ST");
  	startScanST();
  }
  scan.selectST = (i)=>{
  	selectST(i);
  }
  //Private Method

  function Attack(de_dis, isAll) {

  	_function.getEleByID("attack-station-btn").classList.toggle("btn-inactive");
  	_function.getEleByID("attackAllBtn").classList.toggle("btn-inactive");
  	if(isAll) _function.getEleByID("attackAllBtn").textContent = "Attacking...";
  	if(de_dis == "de") _function.getEleByID("DeAttackBtn").textContent = "Attacking...";
  	else _function.getEleByID("DisAttackBtn").textContent ="Attacking...";

  	_function.toggleDisplay("modal", 'show');
  	_function.getEleByID("btnOkTiming").onclick = ()=>{
  		_function.toggleDisplay("modal", 'show');

  		let time = _function.getEleByID("selectTiming").value;
	  	let url = `/deDisAttack.json?time=${time}&` + `type=`; 
	  	url += (de_dis==='de')?"0":"1";
	  	url += isAll?"&all=1":"&all=0";

	  	fetch(url).then(res=>res.json())
	  	.then(data=>{
	  		if(data){
	  			let checkStatus = setInterval(()=>{
	  				fetch("/checkStatus.json").then(res=>res.json())
	  				.then(check=>{
	  					if(!check){
	  						clearInterval(checkStatus);

	  						_function.getEleByID("attack-station-btn").classList.toggle("btn-inactive");
	  						_function.getEleByID("attackAllBtn").classList.toggle("btn-inactive");
	  						if(isAll) _function.getEleByID("attackAllBtn").textContent = "Attack";
	  						if(de_dis == "de") _function.getEleByID("DeAttackBtn").textContent = "Deauthentication ATTACK";
  							else _function.getEleByID("DisAttackBtn").textContent = "Disassociation ATTACK";
	  					}
	  				})
	  			}, 800)
	  		}
	  	})
	  }
  }

  function selectST(i) {
  	fetch("/selectStation.json?id="+i).then(res=>res.json())
  	.then(data=>{
  		if(data){
  			let checkStatus = setInterval(()=>{
  				fetch("/checkStatus.json").then(res=>res.json())
  				.then(check=>{
  					if(!check){
  						clearInterval(checkStatus);
  						if(_function.hasClass("selectSTId"+i, "btn-active")) numSelectST--;
  						else numSelectST++;

  						console.log(numSelectST);
							_function.getEleByID("selectSTId"+i).classList.toggle("btn-active");

							if(numSelectST == 0 || (numSelectST > 0 && !_function.hasClass("attack-station-btn", "show")))
								_function.toggleDisplay("attack-station-btn", 'show');
  					}
  				})
				}, 200)
  		}
  	})
  }

  function startScanST() {
  	_function.getEleByID("scanStationBtn").classList.toggle("btn-inactive");
  	_function.getEleByID("scanStationBtn").textContent = "Scanning...";

  	_function.toggleDisplay("modal", 'show');

  	_function.getEleByID("btnOkTiming").onclick = ()=>{
  		_function.toggleDisplay("modal", 'show');
  		let time = _function.getEleByID("selectTiming").value;
	  	fetch("/startScanStation.json?time="+time).then(res=>{
	  		res.json().then(data=>{
	  			if(data){
	  				let checkStatus = setInterval(()=>{
		  				fetch("/checkStatus.json").then(res=>{
		  					res.json().then(check=>{
		  						if(!check){
		  							clearInterval(checkStatus);
		  							fetch("/requestSendScanStationResults.json").then(res=>{
		  								res.json().then(data=>{
		  									if(data){
		  										let checkStatus1 = setInterval(()=>{
		  											fetch("/checkStatus.json").then(res=>{
		  												res.json().then(check=>{
		  													console.log("checkStatus:", check);
		  													if(!check){
		  														clearInterval(checkStatus1);
		  														fetch("/sendScanStationResults.json").then(res=>{
		  															res.json().then(data=>{

		  																_function.getEleByID("scanStationBtn").classList.toggle("btn-inactive");
  																		_function.getEleByID("scanStationBtn").textContent = "SCAN STATION";

		  																_function.getEleByID("bssidAPSL").innerHTML = data.bssid;
		  																let arData = data.station;
																			console.log("data:", arData);
																			let i = 0, htmlTable = "";
																			for(let data of arData){
																			htmlTable += `
																				<tr>
																					<td>${i+1}</td>
																					<td>${data}</td>
																					<td>
																					<button class="btn-main btn-selectST" id="selectSTId${i}" >Select</button>
																					</td>
																				</tr>
																			`;
																			i++;
																			}
																			document.querySelector('#table-scan-station table thead').innerHTML = `
																				<th>STT</th><th>STATION MAC</th><th>SELECT</th>
																			`;
																			document.querySelector('#table-scan-station table tbody').innerHTML = htmlTable;
		  															})
		  														})
		  													}
		  												})
		  											})
		  										}, 800)
		  									}
		  								})
		  							})
		  						}
		  					})
		  				})
	  				}, 800)
	  			}
	  		})
	  	})
  	}
  }

  function selectAP(i) {
  	fetch("/selectAP.json?id="+i).then(res=>{
  		res.json().then(data=>{
  			if(data){
  				let checkStatus = setInterval(()=>{
  					fetch("/checkStatus.json").then(res=>{
  						res.json().then(check=>{
  							if(!check){
  								clearInterval(checkStatus);
  								var arBtn = document.getElementsByClassName("btn-selectAP");
  								for(let i = 0; i < arBtn.length; i++){
  									arBtn[i].classList.toggle("btn-inactive"); 
  								}
  								_function.getEleByID("selectAPId"+i).classList.toggle("btn-inactive");
  								_function.getEleByID("selectAPId"+i).classList.toggle("btn-active");

  								_function.toggleDisplay("scan-station-btn", 'show');
  							}
  						})
  					})
  				}, 200)
  			}
  		})
  	})
  }

  function startScanAp() {

  	_function.getEleByID("scanAp-btn").classList.toggle("btn-inactive");
  	_function.getEleByID("scanAp-btn").textContent = "Scanning...";

		let rs = fetch('/startScanAP.json');
		rs.then(res=>{
			res.json().then(data=>{
				if(data){
					console.log("startScanAP ", data);
					let checkStatus = setInterval(()=>{
						fetch('/checkStatus.json').then(res=>{
							res.json().then(check=>{
								console.log("checkStatus:", data);
								if(!check){
									clearInterval(checkStatus);
									console.log("START get result");
									fetch('/requestSendScanAPResults.json').then(res1=>{
										res1.json().then(data=>{
											console.log("requestSendScanAPResults:", data);
											if(data){
												let checkStatus1 = setInterval(()=>{
													fetch('/checkStatus.json').then(res=>{
														res.json().then(check=>{
															console.log("checkStatus1:", data);
															if(!check){
																clearInterval(checkStatus1);
																// setTimeout(()=>{
																fetch('/sendScanAPResults.json').then(res=>{
																	res.json().then(data=>{
																		console.log(data);
																		 _function.getEleByID("scanAp-btn").classList.toggle("btn-inactive");
  																	_function.getEleByID("scanAp-btn").textContent = "Scan";

																		 let arData = data.aps;
																		 console.log("data:", arData);
																		 let i = 0, htmlTable = "";
																			
																		 for(let data of arData){
																		 	htmlTable += `
																		 		<tr>
																		 			<td><p>${i+1}</p></td>
																		 			<td><p>${data.n}</p></td>
																		 			<td><p>${data.b}</p></td>
																		 			<td><p>${data.c}</p></td>
																		 			<td><p>${data.r}</p></td>
																		 			<td><p>${data.e}</p></td>
																		 			<td><p>${data.h}</p></td>
																		 			<td>
																		 				<button class="btn-main btn-selectAP" id="selectAPId${i}" >Select</button>
																		 			</td>
																		 		</tr>
																		 	`;
																		 	i++;
																		 }
																		 document.querySelector('#tabel-scan-contain table thead').innerHTML = `
																		 	<tr><th>STT</th><th>ESSID</th><th>BSSID</th><th>CHANEL</th><th>RSSI</th><th>ENCRYPT</th><th>HIDEN</th><th>SELECT</th></tr>
																		 `;
																		 document.querySelector('#tabel-scan-contain table tbody').innerHTML = htmlTable;
																	})
																})
																// }, 800)
															}
														})
													})
												}, 800)
											}
										})

									})
								}
							})
						})
					}, 800)
				}
			})
		})
	}

}( window.scan = window.scan || {}));