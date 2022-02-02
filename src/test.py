loop_dict = {"R1": {"R1": {}, "R2": {}}, "R2": {"R1": {}, "R2": {}}}
test_dict = {"a": {}, "b": [], "c": 1}
print(test_dict["b"])
test_dict["b"].append("c")

secs_tf_dict = {"camera_depth_frame": [], "camera_aligned_depth_to_color_frame": [], "base_link": [],
                "camera_link": {"camera_color_frame": [], "camera_aligned_depth_to_color_frame": [], "camera_depth_frame": []}}
secs_tf_dict["camera_depth_frame"].append("q")
a = "erw"
print(a[1:])
