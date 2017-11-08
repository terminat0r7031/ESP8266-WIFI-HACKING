(function( _beacon ) {
    //Private Property
    var isRandom = false;
    var time = 10;
    var ssid = "";
    var url = "";
    //Public Method
    _beacon.init = () =>{
    	_function.getEleByID("randomATBC").onclick = ()=>{
    		_function.getEleByID("ssid").disabled  = true;
    		isRandom = true;
    	}
    	_function.getEleByID("custumATBC").onclick = ()=>{
    		_function.getEleByID("ssid").disabled  = false;
    		isRandom = false;
    	}
    }
    _beacon.attack = ()=>{
    	time = _function.getEleByID("timeAttackBc").value;
    	ssid = _function.getEleByID("ssid").value;
    	if(isRandom){
    		if(time==""){
    			url = "";
    			alert("Please type time to attack!");
    		}
    		else url = `/beaconAttack.json?random=1&time=${time}`;
    	} else {
    		if(time=="" || ssid == ""){
    			alert("Please type time and ssid to attack!");
    			url = "";
    		} 
    		else
    			url = `/beaconAttack.json?random=1&time=${time}&ssid=${ssid}`;
    	}
    	if(url != ""){
    		alert('ok');
    	}
    }
    //Private Method

}( window._beacon = window._beacon || {}));
_beacon.init();