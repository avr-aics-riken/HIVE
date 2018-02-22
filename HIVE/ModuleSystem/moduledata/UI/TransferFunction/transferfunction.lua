TransferFunction = {}
setmetatable(TransferFunction, {__index = HiveBaseModule})

TransferFunction.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=TransferFunction})
    this.gentex = GenTexture()
    this.loader = LoadModule('ColorMapLoader')
    this.prefilepath = ""
    return this
end

function TransferFunction:Do()
    self:UpdateValue()
    local v = self.value
    
    -- Analyze min/max
    if v.volume and self.vol ~= v.volume then
        self.vol = v.volume
        local analyzer = LoadModule('Analyzer').VolumeAnalyzer()
        analyzer:Execute(v.volume)
        minval = analyzer:MinX()
        maxval = analyzer:MaxX()
        self.property.minval = minval
        self.property.maxval = maxval        
        print('Tranferfunction analyzed: min=', minval, ' max=', maxval)
        local ht = analyzer:GetHistgram()
        local hist = "["
        for a,b in pairs(ht) do
            hist = hist .. string.format("%.4f", b) .. (a ~= #ht and ',' or '')
        end
        hist = hist .. "]"
        
        if targetClientId ~= nil then
            if network then
                local analyzedInfo = [[{
                    "JSONRPC" : "2.0",
                    "method" : "analyzedInfo",            
                    "to" : ]] .. targetClientId ..[[,
                    "id":0,
                    "param" : {
                        "minval" : "]] .. minval .. [[",
                        "maxval" : "]] .. maxval .. [[",
                        "varname": "]] .. self.varname .. [[",
                        "histgram": ]] .. hist .. [[
                    }
                }]]
                --print('json=', analyzedInfo)        
                network:SendJSON(analyzedInfo);
            end
        end
    end
    
    if self.value.filepath ~= nil then
        if self.prefilepath ~= self.value.filepath then
            if self.loader:Load(self.value.filepath) then
                self.prefilepath = self.value.filepath
                self.value.rgba = {}
                local size = self.loader:RGBASize()
                for i = 0, size do
                    table.insert(self.value.rgba, math.floor(self.loader:RGBAValue(i) * 0xFF + 0.5))
                end
                if targetClientId ~= nil then
                    if network then
                        local data = {}
                        data["JSONRPC"] = "2.0"
                        data["method"] = "luaData"
                        data["to"] = targetClientId
                        data["id"] = 0
                        data["param"] = {}
                        data["param"]["name"] = "TransferFunction"
                        data["param"]["type"] = "filedata"
                        data["param"]["rgba"] = self.value.rgba
                        data["param"]["minval"] = self.loader:MinValue()
                        data["param"]["maxval"] = self.loader:MaxValue()
                        network:SendJSON( JSON.encode(data) );
                    end
                end
            end
        end
    end
        
    local rgba = self.value.rgba;
    --print('rgba=', #rgba)
    
    if #rgba == 0 then
        rgba = {}
        local i
        for i = 1, 256 do
            rgba[4*i]   = 255
            rgba[4*i+1] = 0
            rgba[4*i+2] = 0
            rgba[4*i+3] = i-1
        end
    end
    
	self.gentex:Create2D(rgba, 1, 256, 1);
    
    return true
end

function TransferFunction:Image()
    return self.gentex:ImageData()
end

function TransferFunction:Min()
    print('TransferFunction:Min() > ', self.value.minval)
    return self.value.minval
end

function TransferFunction:Max()
    print('TransferFunction:Max() > ', self.value.maxval)
    return self.value.maxval
end

function TransferFunction:RGBA()
    return self.value.rgba
end

function TransferFunction:ColorBar()
    return {
        rgba = self.value.rgba,
        min = self.value.minval,
        max = self.value.maxval,
    };
end
