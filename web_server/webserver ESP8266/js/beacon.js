(function( _beacon ) {
    //Private Property
    var isRamdom = false;
    var time = 10;
    var ssid = "";
    var url = "";
    //Public Method
    _beacon.init = () =>{
    	_function.getEleByID("randomATBC").onclick = ()=>{
    		isRamdom = true;
    		_function.getEleByID("ssid").disabled  = true;
    		_function.getEleByID("ssid").value = "";
    	}
    	_function.getEleByID("custumATBC").onclick = ()=>{
    		isRamdom = false;
    		_function.getEleByID("ssid").disabled  = false;
    	}
    }
    _beacon.attack = ()=>{
    	time = _function.getEleByID("timeAttackBc").value;
    	ssid = _function.getEleByID("ssid").value;

    	if((isRamdom && time != "") || (!isRamdom && time !="" && ssid != "")){
    		url = `/beaconAttack.json?random=${ssid==''?'1':'0'}&time=${time}&ssid=${ssid}`;
    		console.log(url);

    		_function.getEleByID("beacon-attack-btn").classList.toggle("btn-inactive");

    		fetch(url).then(res=>res.json())
    		.then(data=>{
    			if(data){
    				fetch('/checkStatus.json').then(res=>res.json())
					.then(check=>{
						if(!check){
							_function.getEleByID("beacon-attack-btn").classList.toggle("btn-inactive");
						}
					})	
    			}
    		})
    	} else{
    		alert("You can not leave that empty!");
    	}
    }
    //Private Method

}( window._beacon = window._beacon || {}));
_beacon.init();