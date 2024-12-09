import * as THREE from "three";
import { OrbitControls } from "three/addons/controls/OrbitControls.js";
import Stats from "three/addons/libs/stats.module.js";

// https://juejin.cn/post/7305371899138654235

// 链接：从零开始学图形学：MVP Transformation https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F343532009
// 链接：图形学：MVP变换概述 https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F551648397
// 链接：计算机图形学 5：齐次坐标与 MVP 矩阵变换 https://link.juejin.cn/?target=https%3A%2F%2Fzhuanlan.zhihu.com%2Fp%2F261097735

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
const clock = new THREE.Clock();

const one = new THREE.Mesh();
function example1() {
    const vertex = /* GLSL */ `
    uniform float uTime;
    varying vec2 vUv;

    void main() {
      vUv = uv;

      vec3 pos = position * sin(uTime);
      gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
    }
  `;

    const fragment = /* GLSL */ `
  uniform float uTime;
  varying vec2 vUv;

  void main() {
    gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
  }
  `;

    one.geometry = new THREE.SphereGeometry(1);

    one.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        wireframe: true,
    });

    one.position.set(-2.5, 0, 0);
    scene.add(one);
}

example1();

const two = new THREE.Mesh();
function example2() {
    const vertex = /* GLSL */ `
    uniform float uTime;
    varying vec2 vUv;

    void main() {
        vUv = uv;

        vec3 pos = position;
        pos.y += sin(pos.y * (sin(uTime) + 1.0) * 5.0);
        gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);
    }
    `;

    const fragment = /* GLSL */ `
    varying vec2 vUv;
    uniform float uTime;

    void main() {
      gl_FragColor = vec4(1.0, 0.0, 1.0, 1.0);
    }
    `;

    two.geometry = new THREE.SphereGeometry(1);

    two.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        wireframe: true,
    });

    two.position.set(0.0, 0, 0);
    scene.add(two);
}
example2();

const three = new THREE.Mesh();
function example3() {
    const vertex = /* GLSL */ `
    //	Classic Perlin 3D Noise
    //	by Stefan Gustavson
    // https://link.juejin.cn/?target=https%3A%2F%2Fgist.github.com%2Fpatriciogonzalezvivo%2F670c22f3966e662d2f83
    vec4 permute(vec4 x){return mod(((x*34.0)+1.0)*x, 289.0);}
    vec4 taylorInvSqrt(vec4 r){return 1.79284291400159 - 0.85373472095314 * r;}
    vec3 fade(vec3 t) {return t*t*t*(t*(t*6.0-15.0)+10.0);}

    float noise(vec3 P){
        vec3 Pi0 = floor(P); // Integer part for indexing
        vec3 Pi1 = Pi0 + vec3(1.0); // Integer part + 1
        Pi0 = mod(Pi0, 289.0);
        Pi1 = mod(Pi1, 289.0);
        vec3 Pf0 = fract(P); // Fractional part for interpolation
        vec3 Pf1 = Pf0 - vec3(1.0); // Fractional part - 1.0
        vec4 ix = vec4(Pi0.x, Pi1.x, Pi0.x, Pi1.x);
        vec4 iy = vec4(Pi0.yy, Pi1.yy);
        vec4 iz0 = Pi0.zzzz;
        vec4 iz1 = Pi1.zzzz;

        vec4 ixy = permute(permute(ix) + iy);
        vec4 ixy0 = permute(ixy + iz0);
        vec4 ixy1 = permute(ixy + iz1);

        vec4 gx0 = ixy0 / 7.0;
        vec4 gy0 = fract(floor(gx0) / 7.0) - 0.5;
        gx0 = fract(gx0);
        vec4 gz0 = vec4(0.5) - abs(gx0) - abs(gy0);
        vec4 sz0 = step(gz0, vec4(0.0));
        gx0 -= sz0 * (step(0.0, gx0) - 0.5);
        gy0 -= sz0 * (step(0.0, gy0) - 0.5);

        vec4 gx1 = ixy1 / 7.0;
        vec4 gy1 = fract(floor(gx1) / 7.0) - 0.5;
        gx1 = fract(gx1);
        vec4 gz1 = vec4(0.5) - abs(gx1) - abs(gy1);
        vec4 sz1 = step(gz1, vec4(0.0));
        gx1 -= sz1 * (step(0.0, gx1) - 0.5);
        gy1 -= sz1 * (step(0.0, gy1) - 0.5);

        vec3 g000 = vec3(gx0.x,gy0.x,gz0.x);
        vec3 g100 = vec3(gx0.y,gy0.y,gz0.y);
        vec3 g010 = vec3(gx0.z,gy0.z,gz0.z);
        vec3 g110 = vec3(gx0.w,gy0.w,gz0.w);
        vec3 g001 = vec3(gx1.x,gy1.x,gz1.x);
        vec3 g101 = vec3(gx1.y,gy1.y,gz1.y);
        vec3 g011 = vec3(gx1.z,gy1.z,gz1.z);
        vec3 g111 = vec3(gx1.w,gy1.w,gz1.w);

        vec4 norm0 = taylorInvSqrt(vec4(dot(g000, g000), dot(g010, g010), dot(g100, g100), dot(g110, g110)));
        g000 *= norm0.x;
        g010 *= norm0.y;
        g100 *= norm0.z;
        g110 *= norm0.w;
        vec4 norm1 = taylorInvSqrt(vec4(dot(g001, g001), dot(g011, g011), dot(g101, g101), dot(g111, g111)));
        g001 *= norm1.x;
        g011 *= norm1.y;
        g101 *= norm1.z;
        g111 *= norm1.w;

        float n000 = dot(g000, Pf0);
        float n100 = dot(g100, vec3(Pf1.x, Pf0.yz));
        float n010 = dot(g010, vec3(Pf0.x, Pf1.y, Pf0.z));
        float n110 = dot(g110, vec3(Pf1.xy, Pf0.z));
        float n001 = dot(g001, vec3(Pf0.xy, Pf1.z));
        float n101 = dot(g101, vec3(Pf1.x, Pf0.y, Pf1.z));
        float n011 = dot(g011, vec3(Pf0.x, Pf1.yz));
        float n111 = dot(g111, Pf1);

        vec3 fade_xyz = fade(Pf0);
        vec4 n_z = mix(vec4(n000, n100, n010, n110), vec4(n001, n101, n011, n111), fade_xyz.z);
        vec2 n_yz = mix(n_z.xy, n_z.zw, fade_xyz.y);
        float n_xyz = mix(n_yz.x, n_yz.y, fade_xyz.x);
        return 2.2 * n_xyz;
    }

    float random(vec3 pos){
        return fract(sin(dot(pos, vec3(64.25375463, 23.27536534, 86.29678483))) * 59482.7542);
    }

    uniform float uTime;
    varying vec2 vUv;
    varying vec3 vNormal;

    void main() {
        vUv = uv;

        vec3 pos = position;
        pos += normal * noise(position * (sin(uTime) + 1.0) * 4.0);

        gl_Position = projectionMatrix * modelViewMatrix * vec4(pos, 1.0);

        vNormal = normal;
    }
    `;

    const fragment = /* GLSL */ `
    varying vec2 vUv;
    varying vec3 vNormal;
    uniform float uTime;

    void main() {
      gl_FragColor = vec4(vNormal, 1.0);
    }
    `;

    three.geometry = new THREE.SphereGeometry(0.5, 256, 256);

    three.material = new THREE.ShaderMaterial({
        uniforms: {
            uTime: { value: 0 },
        },
        vertexShader: vertex,
        fragmentShader: fragment,
        // wireframe: true,
    });

    three.position.set(3.0, 0, 0);
    scene.add(three);
}
example3();

let time = 0;
function animate() {
    time += 0.01;
    one.material.uniforms.uTime.value = time;
    two.material.uniforms.uTime.value = time;

    const delta = clock.getElapsedTime();
    three.material.uniforms.uTime.value = delta;
    three.rotation.y = delta;

    requestAnimationFrame(animate);
    renderer.render(scene, camera);
}
animate();
