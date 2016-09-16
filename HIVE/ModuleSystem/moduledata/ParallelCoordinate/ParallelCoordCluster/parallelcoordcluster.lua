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
    this.plotAxis = {plotX = -1, plotY = -1}
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

    local axisinfo
    local axisNum = self.value.volume:Component() -- self.volumeclustering:GetAxisNum()
    local ax

    -- check axis info
    local needExe = false
    local needOrder = false
    local axisjson = ""
    local jsonorder = {}
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

        -- オーダーが変更になる場合には Lua でキャッシュしているデータも
        -- 同じようにオーダーを変更してキャッシュしておかないと次に Lua が走るときに
        -- 意味がなくなるので、もしオーダーに変更があった場合にはキャッシュも並び替える
        for ax = 1, axisNum do
            -- json から取得した現在の希望オーダー
            jsonorder[ax] = axisinfo[ax].order
            -- 希望オーダーがnilでなく、かつカウンタの値と異なる場合
            -- 並べ替えが発生しているはずなのでneedexe
            if jsonorder[ax] ~= nil and jsonorder[ax] ~= ax - 1 then
                needExe = true
                needOrder = true
            end
        end
        -- オーダーが変更されていた場合の処理
        if needOrder then
            -- local tempsigma = self.axisSigma
            -- local tempinfo = axisinfo
            -- for ax = 1, axisNum do
            --     self.axisSigma[ax] = tempsigma[jsonorder[ax]]
            --     axisinfo[ax] = tempsigma[jsonorder[ax]]
            -- end
        end

    end

    -- check plot axis prev
    local plotAxisX = self.value.plotX
    local plotAxisY = self.value.plotY
    if self.plotAxis.plotX ~= plotAxisX or self.plotAxis.plotY ~= plotAxisY then
        self.plotAxis.plotX = plotAxisX
        self.plotAxis.plotY = plotAxisY
        needExe = true
    end

    -- check update
    if self.volCache ~= self.value.volume:Pointer() then
        self.volCache = self.value.volume:Pointer()
        needExe = true
    end

    -- execution
    if needExe then
        for ax = 0, axisNum - 1 do
            self.volumeclustering:SetSigma(ax, self.axisSigma[ax+1])
        end
        print('Clustring = ', self.volumeclustering:Execute(self.value.volume))

        -- Plot
        -- [TODO: axis select information]
        local plotX = -1
        local plotY = -1
        if self.value.plotX ~= nil then
            if self.value.plotX > -1 and self.value.plotY > -1 then
                plotX = self.value.plotX
                plotY = self.value.plotY
            end
        end
        self.plot:Execute(self.value.volume, plotX, plotY);
        sendPlot(self.varname, self.plot:GetImageBuffer())
    end


    -- make axis info
    local temp
    local dest = '{'

    dest = dest .. '"volume": {'
    dest = dest .. '  "size":[' .. volWidth .. ', ' .. volHeight .. ',' .. volDepth .. ', '.. volComp .. ' ],'
    dest = dest .. '  "minmax":['
    for ax = 0, axisNum - 1 do
        if ax ~= 0 then
            dest = dest .. ','
        end
        dest = dest .. '{"min":' .. self.volumeclustering:GetVolumeMin(ax) .. ', "max":' .. self.volumeclustering:GetVolumeMax(ax) .. '}'
        --print('{"min":' .. self.volumeclustering:GetVolumeMin(ax) .. ', "max":' .. self.volumeclustering:GetVolumeMax(ax) .. '}')
    end
    dest = dest .. '  ]'
    dest = dest .. ' }'

    dest = dest .. ', "axis": '
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

        if axisjson ~= "" then
            local brushMin = axisinfo[ax+1].brush.min or "null"
            local brushMax = axisinfo[ax+1].brush.max or "null"
            local rangeMin = axisinfo[ax+1].range.min or "null"
            local rangeMax = axisinfo[ax+1].range.max or "null"
            local selectedAxis = "false"
            if axisinfo[ax+1].selectedAxis == true then selectedAxis = "true" end
            dest = dest .. '"brush": {"min": ' .. brushMin .. ', "max": '.. brushMax .. '}, '
            dest = dest .. '"range": {"min": ' .. rangeMin .. ', "max": ' .. rangeMax .. '}, '
            dest = dest .. '"sigma": ' .. self.axisSigma[ax+1] .. ', '
            dest = dest .. '"selectedAxis": ' .. selectedAxis  .. ', '
            dest = dest .. '"selectedNumber": ' .. axisinfo[ax+1].selectedNumber .. ', '
            dest = dest .. '"defaultOrder": ' .. axisinfo[ax+1].defaultOrder .. ', '
        else
            dest = dest .. '"brush": {"min": null, "max": null}, '
            dest = dest .. '"range": {"min": null, "max": null}, '
            dest = dest .. '"sigma": ' .. self.axisSigma[ax+1] .. ', '
            dest = dest .. '"selectedAxis": false, '
            dest = dest .. '"selectedNumber": -1, '
            dest = dest .. '"defaultOrder": ' .. ax .. ', '
        end

        dest = dest .. '"order": ' .. ax .. ', '
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

            if axisjson ~= "" then
                local cColor = ''
                if axisinfo[ax+1].cluster[c+1] == nil then
                    cColor = '[0, 0, 0, 1]'
                else
                    cColor = '[' .. axisinfo[ax+1].cluster[c+1].color[1] .. ', '
                    cColor = cColor .. axisinfo[ax+1].cluster[c+1].color[2] .. ', '
                    cColor = cColor .. axisinfo[ax+1].cluster[c+1].color[3] .. ', '
                    cColor = cColor .. axisinfo[ax+1].cluster[c+1].color[4] .. ']'
                end
                dest = dest .. ', "selected": false, "color": ' .. cColor .. '}'
            else
                dest = dest .. ', "selected": false, "color": [0, 0, 0, 1]}'
            end
        end
        dest = dest .. ']}'
    end

    dest = dest .. ']'


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
