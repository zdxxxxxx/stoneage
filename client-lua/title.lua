function GetTitleStr(id)
	local num = table.getn(titleinfo);
	if id >=1 and id <= num then
		return titleinfo[id]
	else
		return "";
	end
end

function data()
	titleinfo = {
								"[style c=9]��صľ���[/style]",
								"[style c=1]ˮ�ľ���[/style]",
								"[style c=6]���׾���[/style]",
								"[style c=4]���羫��[/style]",
								"[style c=3]�������ľ���[/style]",
								"[style c=1]��[/style][style c=2]��[/style][style c=5]��[/style][style c=3]��[/style][style c=4]��[/style]",
								"[style c=7]����ʹ��[/style]",
								"[style c=7]����������[/style]",
								"[style c=8]·�ߵ���Ҷ[/style]",
								"[style c=9]ˮ���С��[/style]",
								"[style c=10]��૵ĸ���[/style]",
								"[style c=11]���ϵ���Ӱ[/style]",
								"[style c=12]���ܵĴ���[/style]",
								"[style c=13]���ķ���[/style]",
								"[style c=14]ҡҷ�Ľ���[/style]",
								"[style c=15]��ϲ�Ĵ���[/style]",
								"[style c=16]�̺���̫��[/style]",
								"[style c=17]��η�ļž�[/style]",
								"[style c=3]���`��ӡ��[/style]",
								"[style c=2]��ת�ֻ�[/style]",
								"[style c=4]�����ַ���[/style]",
								"[style c=6]��������[/style]",
								"[style c=25]�ᱦ���[/style]",
								"[style c=10]�´��ᱦ��[/style]",
								"[style c=4]�ϳɴ���[/style]",
								"[style c=6]�ϳɸ���[/style]",
								"[style c=4]�������[/style]",
								"[style c=6]�������[/style]",
								"[style c=4]�������[/style]",
								"[style c=6]�������[/style]",
								"[style c=0]����սʿ[/style]",
								"[style c=8]ʥ����ʿ[/style]",
								"[style c=21]������˿Ӣ��[/style]",
								"[style c=23]��ķ����Ӣ��[/style]",
								"[style c=18]������˹ʹ��[/style]",
								"[style c=15]��˹��½Ӣ��[/style]"
							}
end

function main()
	data()
end      



                                                  