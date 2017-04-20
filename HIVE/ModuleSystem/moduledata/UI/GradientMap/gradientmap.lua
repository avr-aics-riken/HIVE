GradientMap = {}
setmetatable(GradientMap, {__index = HiveBaseModule})

GradientMap.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=GradientMap})
    this.gentex = GenTexture()
    this.loader = LoadModule('VisioGradientMapLoader')
    this.prefilepath = ""
    return this
end

function GradientMap:Do()
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
        print('GradientMap analyzed: min=', minval, ' max=', maxval)
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
                self.value.values = {}
                local size = self.loader:GradientSize()
                for i = 0, size do
                   table.insert(self.value.values, math.floor(self.loader:GradientValue(i) * 0xFF + 0.5))
                end
                if targetClientId ~= nil then
                    if network then
                        local data = {}
                        data["JSONRPC"] = "2.0"
                        data["method"] = "luaData"
                        data["to"] = targetClientId
                        data["id"] = 0
                        data["param"] = {}
                        data["param"]["name"] = "GradientMap"
                        data["param"]["type"] = "filedata"
                        data["param"]["values"] = self.value.values
                        data["param"]["minval"] = self.loader:MinValue()
                        data["param"]["maxval"] = self.loader:MaxValue()
                        network:SendJSON( JSON.encode(data) );
                    end
                end
            end
        end
    end
        
    local values = self.value.values;
    --print('values=', #values)
    
    if #values == 0 then
        values = {}
        local i
        for i = 1, 256 do
            values[4*i]   = 255
            values[4*i+1] = 255
            values[4*i+2] = 255
            values[4*i+3] = 255
        end
    end
    
	self.gentex:Create2D(values, 1, 256, 1);
    
    return true
end

function GradientMap:Image()
    return self.gentex:ImageData()
end

function GradientMap:Min()
    print('GradientMap:Min() > ', self.value.minval)
    return self.value.minval
end

function GradientMap:Max()
    print('GradientMap:Max() > ', self.value.maxval)
    return self.value.maxval
end

function GradientMap:RGBA()
    return self.value.values
end
