ParallelCoordCluster = {}
setmetatable(ParallelCoordCluster, {__index = HiveBaseModule})

local JSON = require('dkjson')

ParallelCoordCluster.new = function (varname)
    local this = HiveBaseModule.new(varname)
    setmetatable(this, {__index=ParallelCoordCluster})
    this.gentex = GenTexture()
    this.volumeclustering = require('ClusterParallelCoord').VolumeClustering()
    this.plot = require('ClusterParallelCoord').VolumeScatterPlot();
    this.axisSigma = {}
    local defaultSigma = 0.05
    local maxAxisNum = 20    
    for ax = 1, maxAxisNum do
        this.axisSigma[ax] = defaultSigma
    end
    return this
end

function sendPlot(varname, image)
    if (image == nil) then
        return false
    end
    local mode = 'jpg'
    -- image save
    local imageBuffer
    local imageBufferSize
    -- image save
    imageBuffer = HIVE_ImageSaver:SaveMemory(1, image)
    imageBufferSize = HIVE_ImageSaver:MemorySize()
    
    -- create metabinary
    --local w = v.screensize[1]
    --local h = v.screensize[2]
    local w = image:GetWidth()
    local h = image:GetHeight()
    print('rendersize=('.. w ..",".. h ..")", 'cancel=', tostring(HIVE_isRenderCanceled))
    
    local json = [[{
            "JSONRPC" : "2.0",
            "method" : "renderedImage",            
            "to" : ]] .. targetClientId ..[[,
            "param" : {
                "isplot" : "]] .. "true" .. [[",
                "type" : "]] .. mode .. [[",
                "width" : "]] .. w .. [[",
                "height" : "]] .. h .. [[",
                "varname": "]] .. varname .. [["
            },
            "id":0
    }]]
    HIVE_metabin:Create(json, imageBuffer, imageBufferSize)
    --print('JSON=', json, 'size=', imageBufferSize)
    -- send
    network:SendBinary(HIVE_metabin:BinaryBuffer(), HIVE_metabin:BinaryBufferSize())
end

function sendData(varname, cdata)
    local mode = 'raw'
    local json = [[{
        "JSONRPC" : "2.0",
        "method" : "renderedImage",
        "to" : ]] .. targetClientId ..[[,
        "param" : {
            "varname": "]] .. varname .. [[",
            "mode": "]] .. mode .. [[",
            "data": ]] .. cdata .. [[
        },
        "id":0
    }]]
    network:SendJSON(json)
end


function ParallelCoordCluster:Do()
    self:UpdateValue()
    -- generate selection texture
    self.gentex:Create2D(self.value.rgba, 1, 256, 1);
    --self:PrintValue()

    if self.value.volume == nil then
        return 'No volume input'
    end

    local volWidth = self.value.volume:Width()
    local volHeight = self.value.volume:Height()
    local volDepth = self.value.volume:Depth()
    local volComp = self.value.volume:Component()

    self.gentex:Create2D(self.value.rgba, 1, 256, volComp * 2);

    local axisNum = self.value.volume:Component() -- self.volumeclustering:GetAxisNum()
    local ax
    

    -- check axis info
    local needExe = false
    local axisjson = ""
    if self.value.axisjson ~= "" then -- axis info edited?
        axisjson = self.value.axisjson

        axisinfo = JSON.decode(axisjson)
        for ax = 1, axisNum do
            local axSigma = axisinfo[ax].sigma
            if axSigma ~= nil and self.axisSigma[ax] ~= axSigma then
                self.axisSigma[ax] = axSigma
                needExe = true
            end
        end
    end

    -- check update
    if self.volCache ~= self.value.volume:Pointer() then
        self.volCache = self.value.volume:Pointer()
        needExe = true
    end
    
    -- execution
    if needExe then
        for ax = 0, axisNum-1 do
            self.volumeclustering:SetSigma(ax, self.axisSigma[ax+1])
        end
        print('Clustring = ', self.volumeclustering:Execute(self.value.volume))

        -- Plot
        -- [TODO: axis select information]
        self.plot:Execute(self.value.volume, 0, 1); 
        sendPlot(self.varname, self.plot:GetImageBuffer())
    end


    -- make axis info    
    local temp
    local dest = '{'
    
    dest = dest .. '"volume": {'
    dest = dest .. '  "size":[' .. volWidth .. ', ' .. volHeight .. ',' .. volDepth .. ', '.. volComp .. ' ],'
    dest = dest .. '  "minmax":['
    for  ax = 0, axisNum-1 do
        if ax ~= 0 then
            dest = dest .. ','
        end
        dest = dest .. '{"min":' .. self.volumeclustering:GetVolumeMin(ax) .. ', "max":' .. self.volumeclustering:GetVolumeMax(ax) .. '}'
        --print('{"min":' .. self.volumeclustering:GetVolumeMin(ax) .. ', "max":' .. self.volumeclustering:GetVolumeMax(ax) .. '}')
    end
    dest = dest .. '  ]'
    dest = dest .. ' }'

    dest = dest .. ', "axis": '
    if axisjson ~= "" then
        dest = dest .. axisjson
    else
        dest = dest .. '['
        --print('AxisNum = ' .. axisNum)
        for ax = 0, axisNum - 1 do
            local cnum = self.volumeclustering:GetClusterNum(ax)
            print('ClusterNum = ' .. cnum)

            -- json string
            if ax == 0 then
                dest = dest .. '{'
            else
                dest = dest .. ',{'
            end
            dest = dest .. '"title": "title_' .. ax .. '", '
            dest = dest .. '"brush": {"min": null, "max": null}, '
            dest = dest .. '"range": {"min": null, "max": null}, '
            dest = dest .. '"sigma": ' .. self.axisSigma[ax+1] .. ', '
            dest = dest .. '"selectedNumber": -1, '
            dest = dest .. '"clusternum": ' .. cnum .. ', '
            dest = dest .. '"cluster": ['
            for c = 0, cnum - 1 do
                if c == 0 then
                    dest = dest .. '{'
                else
                    dest = dest .. ',{'
                end
                local cv = self.volumeclustering:GetClusterValue(ax, c)
                local j = 0
                for i,v in pairs(cv) do
                    temp = string.gsub(i, 'Value', '');
                    if j ~= 0 then
                        dest = dest .. ','
                    end
                    j = j + 1
                    dest = dest .. '"' .. temp .. '": ' .. v
                end
                dest = dest .. ', "selected": false, "color": [0, 0, 0, 1]}'
            end
            dest = dest .. ']}'
        end

        dest = dest .. ']'
    end

    --- make Edge info
    local datanum = volWidth * volHeight * volDepth
    dest = dest .. ', "edge": {"volumenum": ' .. datanum .. ', "cluster": ['
    for ax = 0, axisNum - 2 do
        local cnum1 = self.volumeclustering:GetClusterNum(ax)
        local cnum2 = self.volumeclustering:GetClusterNum(ax + 1)
        if ax == 0 then
            dest = dest .. '['
        else
            dest = dest .. ',['
        end
        for c1 = 0, cnum1 - 1 do
            if c1 == 0 then
                dest = dest .. '['
            else
                dest = dest .. ',['
            end
            temp = ""
            for c2 = 0, cnum2 - 1 do
                if c2 ~= 0 then
                    temp = temp .. ','
                end
                local cnt = self.volumeclustering:GetEdgePowers(ax, c1, c2)
                temp = temp .. cnt
                --if cnt > 0 then
                --    print(ax .."-"..  c1 .. " <-> " .. ax + 1 .."-"..  c2 .. " = " .. cnt / datanum)
                --end
            end
            dest = dest .. temp .. "]"
        end
        dest = dest .. "]"
    end

    dest = dest .. ']}}'

    sendData(self.varname, dest)
    
    return true
end

function ParallelCoordCluster:select()
    --self:UpdateValue()
    return self.gentex:ImageData()
end
