clear;
clc;
ID_path='D:\BaiduYunDownload\label_face\data';
output_file='D:\BaiduYunDownload\label_face\label.txt';
start_id = input('Start_id : ');
global fid;
if start_id > 1
    fid = fopen(output_file, 'a');
else
    fid = fopen(output_file, 'w');
end
for i = start_id : 3786
    fprintf('id = %d : \n', i);
    im = imread(sprintf('%s/%d/%d.bmp', ID_path, i,1));
    sz_x = size(im, 1);
    sz_y = size(im, 2);
    space = 10;% 
    blank = ones(sz_y, space, 3) * 250;
    for j = 2 : 18
        img = imread(sprintf('%s/%d/%d.bmp', ID_path, i, j));
        im = [im, blank, img];
    end
    imshow(im);
    im_x = size(im, 2);
    [x, y] = ginput(1);
    a = 0;
    while y < sz_y 
        if (mod(x, sz_x + space) <= sz_x && y >= 0 && x > 0 && x < im_x)
            a = floor(x / (sz_x + space) + 1);
            break;
        end
        fprintf('Try Again Point 1\n');
        [x, y] = ginput(1);
    end
    %if a <= 0 || a > 18
    %    fprintf('Skip ID %d \n', i);
    %    continue;
    %end
    fprintf(' --- Point1 : %d\n', a);
    [x, y] = ginput(1);
    b = 0;
    while y < sz_y
        if (mod(x, sz_x + space) <= sz_x && y >= 0 && x > 0 && x < im_x)
            b = floor(x / (sz_x + space) + 1);
            break;
        end
        [x, y] = ginput(1);
        fprintf('Try Again Point 2\n');
    end
    if b <= 0 || b > 18
        fprintf('Skip ID %d \n', i);
        continue;
    end
    fprintf(' --- Point2 : %d\n', b);
    if a < b
        fprintf(fid, '%d\t%d\t%d\r\n', i, a, b);
    else
        fprintf(fid, '%d\t%d\t%d\r\n', i, b, a);
    end
end
fclose(fid);
