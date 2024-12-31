function getIntPart(x)
    if x <= 0 then
       return math.ceil(x);
    end

    if math.ceil(x) == x then
       x = math.ceil(x);
    else
       x = math.ceil(x) - 1;
    end
    return x;
end

function lua_string_split(str, split_char)
    local sub_str_tab = {};
    while (true) do
        local pos = string.find(str, split_char);
        if (not pos) then
            sub_str_tab[#sub_str_tab + 1] = str;
            break;
        end
        local sub_str = string.sub(str, 1, pos - 1);
        sub_str_tab[#sub_str_tab + 1] = sub_str;
        str = string.sub(str, pos + 1, #str);
    end

    return sub_str_tab;
end

function WindowInit(strdata)
	winW = 6
	winH = 4
	winX = (800 - winW*64)/4
	winY = (600 - winH*48)/4
	if addr ~= 0 then
		sa.DeathAction(addr)
		addr = 0
	end
	data()
	vippoint = tonumber(strdata);
	win.InitStrBuffer(winX + 180,winY + 180,6,0,1);

end

function CreateWinType()
	if addr == 0 then
		addr = sa.MakeWindowDisp(winX,winY,winW, winH, 0, 1)
	end
	sa.PlaySe(202, 320, 240)
	return addr;
end

function ServerWindowType()
	win.SetInputMsgNum()

	sa.StockFontBuffer(winX + 230,winY + 80, 1, 10, "���������", 0)
	local id ;
	local color={};

	local posy = 0

	sa.StockFontBuffer(winX + 100,winY + 100 + posy, 1, 0, "��ǰ��ң�", 0)

	sa.StockFontBuffer(winX + 180,winY + 100 + posy, 1, 0,"".. vippoint, 0);

	sa.StockFontBuffer(winX + 100,winY + 120 + posy, 1, 0, "����ģʽ��", 0);


	if familymode == 0 then
		color[1]=1;
		color[2]=0;
		color[3]=0;
	elseif familymode == 1 then
		color[1]=0;
		color[2]=1;
		color[3]=0;
	elseif familymode == 2 then
		color[1]=0;
		color[2]=0;
		color[3]=1;
	end

	id = sa.StockFontBuffer(winX + 180,winY + 120 + posy, 1, color[1], "����", 2);

	if win.getHitFontNo() == id then
		win.ShowBottomLineString(0,"ֻ�м����Ա������ȡ��");
		if win.getMouseLeftCrick() > 0 then
			familymode=0;
		end
	end
	id = sa.StockFontBuffer(winX + 240,winY + 120 + posy, 1, color[2], "ȫ��", 2);
	if win.getHitFontNo() == id then
		win.ShowBottomLineString(0,"ȫ����Ҷ�����ȡ��");
		if win.getMouseLeftCrick() > 0 then
			familymode=1;
		end
	end

	sa.StockFontBuffer(winX + 100,winY + 140 + posy, 1, 0, "��ȡ��ʽ��", 0);

	if getmode == 0 then
		color[1]=1;
		color[2]=0;
	else
		color[1]=0;
		color[2]=1;
	end

	id = sa.StockFontBuffer(winX + 180,winY + 140 + posy, 1, color[1], "ƴ����", 2);
	if win.getHitFontNo() == id then
		win.ShowBottomLineString(0,"�����ȡ�ĺ����ƴ������");
		if win.getMouseLeftCrick() > 0 then
			getmode=0;
		end
	end
	id = sa.StockFontBuffer(winX + 240,winY + 140 + posy, 1, color[2], "ƽ��", 2);
	if win.getHitFontNo() == id then
		win.ShowBottomLineString(0,"�����ȡ�ĺ��ÿ����һ����");
		if win.getMouseLeftCrick() > 0 then
			getmode=1;
		end
	end

	sa.StockFontBuffer(winX + 100,winY + 160 + posy, 1, 0, "���������", 0);

	id = sa.StockFontBuffer(winX + 180,winY + 160 + posy, 1, 0, "           ", 2);
	if win.getHitFontNo() == id then
		if win.getMouseLeftCrick() > 0 then
			win.InitStrBuffer(winX + 180,winY + 160,2,0,1);
			win.GetKeyInputFocus();
			Focus=1;
		end
	end


	if Focus == -1 then
		sa.StockFontBuffer(winX + 180,winY + 160 + posy, 1, 0,rednum , 2);
		sa.StockFontBuffer(winX + 180,winY + 180 + posy, 1, 0,redValue, 0);
	elseif Focus == 1 then
		sa.StockFontBuffer(winX + 180,winY + 180 + posy, 1, 0,redValue, 0);
		if string.len(win.GetInputMsg()) > 0 then
			rednum = tonumber(win.GetInputMsg());
		end
	elseif Focus == 2 then
		if string.len(win.GetInputMsg()) > 0 then
			redValue = tonumber(win.GetInputMsg());
		end
		sa.StockFontBuffer(winX + 180,winY + 160 + posy, 1, 0,rednum , 2);
	end


	sa.StockFontBuffer(winX + 100,winY + 180 + posy, 1, 0, "��  ��  �", 0);
	id = sa.StockFontBuffer(winX + 180,winY + 180 + posy, 1, 0, "           ", 2);
	if win.getHitFontNo() == id then
		if win.getMouseLeftCrick() > 0 then
			win.InitStrBuffer(winX + 180,winY + 180,6,0,1);
			win.GetKeyInputFocus();
			Focus=2;
		end
	end
	if getmode==0 then
		sa.StockFontBuffer(winX + 300,winY + 180 + posy, 1, 4, "��ǰΪƴ�������", 0);
	else
		sa.StockFontBuffer(winX + 300,winY + 180 + posy, 1, 4, "��ǰΪƽ�����", 0);
	end

	id = sa.StockFontBuffer( winX + 180 , winY + 210 + posy, 1, 4, "�������", 2);
	if win.getHitFontNo() == id then
		if win.getMouseLeftCrick() > 0 then
			if rednum > 0 then
				if redValue > 0 and redValue >= rednum * 10 then
					if redValue <= vippoint then
						local token = familymode.."|"..getmode.."|"..rednum.."|"..redValue
						lssproto.windows(win.getIndexWN(), win.getIdWN(),1,token);
						win.CloseWindow()
					else
						sa.ShowMessage("����ǰ�Ľ�Ҳ��㣡",4)
					end
				else
					sa.ShowMessage("�����С����Ϊ0�Ҵ��ڻ���ں��������10����",4)
				end
			else
				sa.ShowMessage("�����������Ϊ0��",4)
			end
		end
	end
	id = sa.StockFontBuffer(winX + 310,winY + 210 + posy, 1, 4, "��    ��", 2);
	if win.getHitFontNo() == id then
		if win.getMouseLeftCrick() > 0 then
			win.CloseWindow()
		end
	end
	win.StockFontBuffer2();
	if win.DelWindow() == 1 then
		sa.DeathAction(addr)
		win.CloseFlg()
		addr = 0;
		sa.GetKeyInputFocus()
		win.CloseWindow()
	end
	return addr;
end


function data()
	addr=0;
	familymode=0;
	getmode=0;
	rednum=0;
	redValue=0;
	Focus=-1;
end


function main()
	data()
end
