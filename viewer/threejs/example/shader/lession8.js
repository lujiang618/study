import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";
// import { GUI } from "three/addons/libs/lil-gui.module.min.js";
import GUI from "https://cdn.jsdelivr.net/npm/lil-gui@0.19/+esm";

// https://juejin.cn/post/7305371899138654235

// 链接：从零开始学图形学：MVP Transformation https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F343532009
// 链接：图形学：MVP变换概述 https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F551648397
// 链接：计算机图形学 5：齐次坐标与 MVP 矩阵变换 https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F261097735

/**
 * 使旋转的角度随顶点坐标里的x值而变化，而不是所有顶点都用统一的数值。
 * 随着细分数增加、顶点数增多，中间顶点随着x坐标的变化以不同角度发生旋转，从而有了丝滑的扭曲效果
 */
const renderer = new THREE.WebGLRenderer({
    antialias: true, // 抗锯齿
});
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

const camera = new THREE.PerspectiveCamera(
    45,
    window.innerWidth / window.innerHeight,
    1,
    500
);
camera.position.set(0, 0, 5);

const scene = new THREE.Scene();
scene.background = new THREE.Color(0x8899ff);

const controls = new OrbitControls(camera, renderer.domElement);
const stats = new Stats();
const gui = new GUI();
const clock = new THREE.Clock();

const three = new THREE.Mesh();
function example3() {
    const vertexShader = /* GLSL */ `
    mat4 rotationMatrix(vec3 axis, float angle) {
        axis = normalize(axis);
        float s = sin(angle);
        float c = cos(angle);
        float oc = 1.0 - c;

        return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                    oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                    oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                    0.0,                                0.0,                                0.0,                                1.0);
    }

    vec3 rotate(vec3 v, vec3 axis, float angle) {
        mat4 m = rotationMatrix(axis, angle);
        return (m * vec4(v, 1.0)).xyz;
    }

    uniform float uTime;
    varying vec2 vUv;
    const float PI = 3.1415925;
    void main() {
        vUv = uv;
        vec3 axis = vec3(1.0, 0.0, 0.0);
        float angle = position.x * sin(uTime) - uTime;
        vec3 pos = rotate(position, axis, angle);
        gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
    }
    `;

    const fragmentShader = /* GLSL */ `
    varying vec2 vUv;

    void main() {
        vec3 color1 = vec3(0.847, 0.204, 0.373);
        vec3 color2 = vec3(1.0);

        float mixer = step(0.5, fract(vUv.y * 3.0));
        vec3 color = mix(color1, color2, mixer);
        gl_FragColor = vec4(color, 1.0);
    }
    `;

    three.geometry = new THREE.BoxGeometry(3, 1, 1, 64, 64, 64);

    three.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
            uStrength: { value: 0.5 },
            uFrequency: { value: 5.0 },
        },
        vertexShader: vertexShader,
        fragmentShader: fragmentShader,
        // wireframe: true,
    });

    three.position.set(0.0, 0, 0);
    scene.add(three);
}
example3();

gui.add(three.material.uniforms.uStrength, "value", 0, 1, 0.01).name(
    "uStrength"
);

gui.add(three.material.uniforms.uFrequency, "value", 0, 20, 0.01).name(
    "uFrequency"
);

let time = 0;
function animate() {
    time += 0.01;
    const delta = clock.getElapsedTime();
    three.material.uniforms.uTime.value = delta;
    // three.rotation.y = time;

    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();
